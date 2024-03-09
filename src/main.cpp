#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <iostream>
#include <string>
#include <vector>
#include "../include/config.hpp"

void resetBoard(int* board, int n) {
  for (int i = 0; i < n * n * n; i++) {
    *(board + i) = 0;
  }
}

int getNextHighestBlock(int* board, int n, int x, int z) {
  int hi = -1;

  for (int j = 0; j < n; j++) {
    int v = *(board + x * n * n + j * n + z);
    if (v == 1 || v == 2) {
      hi = j;
    }
  }

  return hi + 1;
}

void placeBlock(int* board, int n, int x, int z, int p) {
  int highest = getNextHighestBlock(board, n, x, z);

  if (highest >= n || x >= n || z >= n || x < 0 || z < 0) return;

  *(board + x * n * n + highest * n + z) = p;
}

void renderBoard(int* board, int n, Vector2 sel, float s, int g) {
  //Render Grid
  Color gridColor = Color{255, 255, 255, 255};
  for (float i = 0; i < n; i++) {
    for (float j = 0; j < n; j++) {
      DrawLine3D({i * s, 0.0f, j * s}, {i * s + s, 0.0f, j * s}, gridColor);
      DrawLine3D({i * s, 0.0f, j * s}, {i * s, 0.0f, j * s + s}, gridColor);
      DrawLine3D({i * s + s, 0.0f, j * s}, {i * s + s, 0.0f, j * s + s}, gridColor);
      DrawLine3D({i * s, 0.0f, j * s + s}, {i * s + s, 0.0f, j * s + s}, gridColor);
    }
  }

  //Render Cubes
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        int v = *(board + i * n * n + j * n + k);
        Vector3 pos = {s / 2.0f + i * s, s / 2.0f + j * s, s / 2.0f + k * s};
        if (!g) {
          if (v == 1 || v == 2) {
            DrawCube(pos, s, s, s, (v == 1 ? Color{255, 0, 0, 45} : Color{0, 0, 255, 45}));
            DrawCubeWires(pos, s, s, s, (v == 1 ? Color{255, 0, 0, 255} : Color{0, 0, 255, 255}));
          }
        } else {
          if (v == 3 || v == 4) {
            DrawCube(pos, s, s, s, (v == 3 ? Color{255, 155, 0, 85} : Color{0, 255, 255, 45}));
            DrawCubeWires(pos, s, s, s, (v == 3 ? Color{255, 155, 0, 255} : Color{0, 255, 255, 255}));
          } else if (v == 1 || v == 2) {
            DrawCube(pos, s, s, s, (v == 1 ? Color{255, 0, 0, 45} : Color{0, 0, 255, 45}));
            DrawCubeWires(pos, s, s, s, (v == 1 ? Color{255, 0, 0, 255} : Color{0, 0, 255, 255}));
          }
        }
      }
    }
  }

  if (!g) {
    Vector3 pos = {sel.x * s + s / 2.0f, s / 2.0f + getNextHighestBlock(board, n, sel.x, sel.y) * s, sel.y * s + s / 2.0f};
    DrawCube(pos, s, s, s, {255, 255, 255, 55});
    DrawCubeWires(pos, s, s, s, {255, 255, 255, 255});
  }
}

bool checkWinner(int* board, int n, int p, int x) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        if (*(board + i * n * n + j * n + k) == p) {
          int c = 0;
          std::vector<int> toOutline;

          //Horizontal
          do {
            if (*(board + (c + i) * n * n + j * n + k) == p) c++;
            else c = 0;

            toOutline.push_back((c + i - 1) * n * n + j * n + k);

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            };
          } while (c);

          toOutline.clear();

          //Vertical
          do {
            if (*(board + i * n * n + j * n + k + c) == p) c++;
            else c = 0;

            toOutline.push_back(i * n * n + j * n + k + c - 1);

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            }
          } while (c);

          toOutline.clear();

          //Diagonal ascending plane
          do {
            if (*(board + (i - c) * n * n + j * n + k + c) == p) c++;
            else c = 0;

            toOutline.push_back((i - c + 1) * n * n + j * n + k + c - 1);

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            }
          } while (c);

          toOutline.clear();

          //Diagonal descending plane
          do {
            if (*(board + (i + c) * n * n + j * n + k + c) == p) c++;
            else c = 0;

            toOutline.push_back((i + c - 1) * n * n + j * n + k + c - 1);

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            };
          } while (c);

          toOutline.clear();

          //Up
          do {
            if (*(board + i * n * n + (j + c) * n + k) == p) c++;
            else c = 0;

            toOutline.push_back(i * n * n + (j + c -1) * n + k);

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            }
          } while (c);

          toOutline.clear();

          //3D diag 1
          do {
            if (*(board + (i + c) * n * n + (j + c) * n + k) == p) c++;
            else c = 0;

            toOutline.push_back((i + c - 1) * n * n + (j + c - 1) * n + k);

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            }
          } while (c);

          toOutline.clear();

          //3D diag 2
          do {
            if (*(board + (i - c) * n * n + (j + c) * n + k) == p) c++;
            else c = 0;

            toOutline.push_back((i - c + 1) * n * n + (j + c - 1) * n + k);

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            }
          } while (c);

          toOutline.clear();

          //3D diag 3
          do {
            if (*(board + i * n * n + (j + c) * n + (k + c)) == p) c++;
            else c = 0;

            toOutline.push_back(i * n * n + (j + c - 1) * n + k + c - 1);

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            }
          } while (c);

          toOutline.clear();

          //3D diag 4
          do {
            if (*(board + i * n * n + (j + c) * n + (k - c)) == p) c++;
            else c = 0;

            toOutline.push_back(i * n * n + (j + c - 1) * n + (k - c + 1));

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            }
          } while (c);

          toOutline.clear();

          //3D diag 5
          do {
            if (*(board + (i - c) * n * n + (j + c) * n + (k - c)) == p) c++;
            else c = 0;

            toOutline.push_back((i - c + 1) * n * n + (j + c - 1) * n + (k - c + 1));

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            }
          } while (c);

          toOutline.clear();

          //3D diag 6
          do {
            if (*(board + (i + c) * n * n + (j + c) * n + (k - c)) == p) c++;
            else c = 0;

            toOutline.push_back((i + c - 1) * n * n + (j + c - 1) * n + k - c + 1);

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            }
          } while (c);

          toOutline.clear();

          //3D diag 7
          do {
            if (*(board + (i - c) * n * n + (j + c) * n + (k + c)) == p) c++;
            else c = 0;

            toOutline.push_back((i - c + 1) * n * n + (j + c - 1) * n + k + c -1);

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            }
          } while (c);

          toOutline.clear();

          //3D diag 8
          do {
            if (*(board + (i + c) * n * n + (j + c) * n + (k + c)) == p) c++;
            else c = 0;

            toOutline.push_back((i + c - 1) * n * n + (j + c - 1) * n + k + c - 1);

            if (c == x) {
              for (int i : toOutline) {
                *(board + i) = p + 2;
              }
              return 1;
            }
          } while (c);
        }
      }
    }
  }
  return 0;
}

int getBoardState(int* board, int n, int x) {
  if (checkWinner(board, n, 1, x)) return 1;
  else if (checkWinner(board, n, 2, x)) return 2;
  return 0;
}

int main() {
  std::cout << "ConnectX 3D by danimania" << std::endl << std::endl;

  Vector2 sel = {0, 0};

  float s = 5.0f;

  int g = 0;

  int n;
  std::cout << "Board size: ";
  std::cin >> n;

  int x;
  std::cout << "Number of connections in order to win: ";
  std::cin >> x;

  int* board = new int[n * n * n];
  resetBoard(board, n);

  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ConnectX 3D");

  SetTargetFPS(60);

  float f = 0.2f;

  float cSpeed = 2.0f;
  float cZoom = 1.0f;
  Camera3D* camera = new Camera3D();
  camera->position = (Vector3) {40.0f * n * f, 15.0f * n * f * 1.5f, 40.0f * n * f};
  camera->target = (Vector3){0.0f, 0.0f, 0.0f};
  camera->up = (Vector3){0.0f, 1.0f, 0.0f};
  camera->fovy = 60.0f;
  camera->projection = CAMERA_PERSPECTIVE;

  int p = 1;
  while(!WindowShouldClose()) {
    //Handle Input
    if (!g) {
      if (IsKeyPressed(KEY_W)) {
        sel.x--;
        if (sel.x < 0) sel.x = 0;
      } else if (IsKeyPressed(KEY_S)) {
        sel.x++;
        if (sel.x >= n) sel.x = n - 1;
      } else if (IsKeyPressed(KEY_A)) {
        sel.y++;
        if (sel.y >= n) sel.y = n - 1;
      } else if (IsKeyPressed(KEY_D)) {
        sel.y--;
        if (sel.y < 0) sel.y = 0;
      } else if (IsKeyPressed(KEY_SPACE)) {
        placeBlock(board, n, sel.x, sel.y, p);
        p = 3 - p;
      }
    }

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) {
      Matrix rotation = MatrixRotate(Vector3Normalize(camera->up), cSpeed * GetFrameTime() * (IsKeyDown(KEY_RIGHT) ? 1 : -1));
      Vector3 view = Vector3Subtract(camera->position, camera->target);
      view = Vector3Transform(view, rotation);
      camera->position = Vector3Add(camera->target, view);
    } else if (IsKeyDown(KEY_UP)) {
      camera->position = Vector3Add(camera->position, {-cZoom, -cZoom, -cZoom});
    } else if (IsKeyDown(KEY_DOWN)) {
      camera->position = Vector3Add(camera->position, {cZoom, cZoom, cZoom});
    } else if (IsKeyDown(KEY_R)) {
      camera->position = (Vector3) {40.0f * n * f, 15.0f * n * f * 1.5f, 40.0f * n * f};
      camera->target = (Vector3){0.0f, 0.0f, 0.0f};
    }
    //UpdateCamera(camera, CAMERA_PERSPECTIVE);

    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode3D(*camera);

    renderBoard(board, n, sel, s, g);

    EndMode3D();

    std::string moveString = "Player " + std::to_string(p) + " moves";
    if (g) {
      moveString = "Player " + std::to_string(g) + " wins!";
    }
    DrawText(moveString.c_str(), SCREEN_WIDTH / 2.0f - MeasureText(moveString.c_str(), 30.0f) / 2.0f, SCREEN_HEIGHT - 50.0f, 30.0f, {255, 255, 255, 255});

    EndDrawing();

    if (g == 0) g = getBoardState(board, n, x);
  }

  CloseWindow();
  return 0;
}

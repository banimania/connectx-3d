#include <raylib.h>
#include <rlgl.h>
#include <iostream>
#include "../include/config.hpp"

void resetBoard(int* board, int n) {
  for (int i = 0; i < n * n * n; i++) {
    *(board + i) = 0;
  }
}

int getNextHighestBlock(int* board, int n, int x, int z) {
  int hi = -1;

  float h = n / 2.0f;
  for (int i = -h; i < h; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = -h; k < h; k++) {
        if (x == i && z == k && j > hi) {
          if (board[i * n * n + j * n + k] != 0) {
            hi = j;
          }
        }
      }
    }
  }
  return hi + 1;
}

void placeBlock(int* board, int n, int x, int z, int p) {
  float h = n / 2.0f;

  int highest = getNextHighestBlock(board, n, x, z);

  if (highest >= n) return;

  *(board + x * n * n + highest * n + z) = p;
}

void renderBoard(int* board, int n, Vector2 sel) {
  //Render Grid

  float h = n / 2.0f;
  float s = 5.0f;

  rlBegin(RL_LINES);

  for (float i = -h; i <= h; i++) {
    rlColor3f(0.75f, 0.75f, 0.75f);
    rlColor3f(0.75f, 0.75f, 0.75f);
    rlColor3f(0.75f, 0.75f, 0.75f);
    rlColor3f(0.75f, 0.75f, 0.75f);

    rlVertex3f((float) i * s, 0.0f, (float) -h * s);
    rlVertex3f((float) i * s, 0.0f, (float) h * s);
    rlVertex3f((float) -h * s, 0.0f, (float) i * s);
    rlVertex3f((float) h * s, 0.0f, (float) i * s);
  }

  rlEnd();

  //Render Cubes

  for (float i = -h + 0.5f; i < h; i++) {
    for (float j = 0; j < n; j++) {
      for (float k = -h + 0.5f; k < h; k++) {
        int v = *(board + (int) i * n * n + (int) j * n + (int) k);
        if (v == 1 || v == 2) {
          Vector3 pos = {i * s, 2.5f + j * s, (float) k * s};
          DrawCube(pos, s, s, s, (v == 1 ? Color{255, 0, 0, 45} : Color{0, 0, 255, 45}));
          DrawCubeWires(pos, s, s, s, (v == 1 ? Color{255, 0, 0, 255} : Color{0, 0, 255, 255}));
        }
      }
    }
  }

  Vector3 pos = {sel.x * s, 2.5f + getNextHighestBlock(board, n, sel.x, sel.y) * s, sel.y * s};
  DrawCube(pos, s, s, s, {255, 255, 255, 55});
  DrawCubeWires(pos, s, s, s, {255, 255, 255, 255});
}

int main() {
  std::cout << "ConnectX 3D by danimania" << std::endl << std::endl;

  Vector2 sel = {0, 0};

  int n;
  std::cout << "Board size: ";
  std::cin >> n;

  int* board = new int[n * n * n];
  resetBoard(board, n);

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ConnectX 3D");

  SetTargetFPS(60);

  float f = 0.2f;

  Camera3D* camera = new Camera3D();
  camera->position = (Vector3) {30.0f * n * f, 20.0f * n * f * 1.5f, 30.0f * n * f};
  camera->target = (Vector3){0.0f, 0.0f, 0.0f};
  camera->up = (Vector3){0.0f, 1.0f, 0.0f};
  camera->fovy = 60.0f;
  camera->projection = CAMERA_PERSPECTIVE;

  int p = 1;
  while(!WindowShouldClose()) {
    //Handle Input
    if (IsKeyPressed(KEY_W)) {
      sel.x--;
    } else if (IsKeyPressed(KEY_S)) {
      sel.x++;
    } else if (IsKeyPressed(KEY_A)) {
      sel.y++;
    } else if (IsKeyPressed(KEY_D)) {
      sel.y--;
    } else if (IsKeyPressed(KEY_SPACE)) {
      placeBlock(board, n, sel.x, sel.y, p);
      p = 3 - p;
    }

    //UpdateCamera(camera, CAMERA_PERSPECTIVE);

    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode3D(*camera);

    renderBoard(board, n, sel);

    EndMode3D();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

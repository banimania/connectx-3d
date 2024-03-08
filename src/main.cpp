#include <raylib.h>
#include <rlgl.h>
#include <iostream>
#include "../include/config.hpp"

void resetBoard(int* board, int n) {
  for (int i = 0; i < n * n * n; i++) {
    board[i] = 0;
  }
}

int getNextHighestBlock(int* board, int n, int x, int y) {
  int h = -1;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        if (x == i && y == k && j > h) {
          if (board[i * n * n + j * n + k] != 0) {
            h = j;
          }
        }
      }
    }
  }
  return h + 1;
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

  
  DrawCube({sel.x * s, 2.5f + getNextHighestBlock(board, n, sel.x, sel.y) * s, sel.y * s}, s, s, s, {255, 255, 255, 55});
  DrawCubeWires({sel.x * s, 2.5f + getNextHighestBlock(board, n, sel.x, sel.y) * s, sel.y * s}, s, s, s, {255, 255, 255, 255});

  //Draw Cubes
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int k = 0; k < n; k++) {
        int v = board[i * n * n + j * n + k];
        if (v != 0) {
          DrawCube({(float) i * (n * n) * s, (float) j * n * s + 2.5f, (float) k * s}, s, s, s, (v == 1 ? Color{255, 0, 0, 55} : Color{0, 0, 255, 55}));
          DrawCubeWires({(float) i * (n * n) * s, (float) j * n * s + 2.5f, (float) k * s}, s, s, s, (v == 1 ? Color{255, 0, 0, 255} : Color{0, 0, 255, 255}));
        }
      }
    }
  }
}

int main() {
  std::cout << "ConnectX 3D by danimania" << std::endl << std::endl;

  Vector2 sel = {0, 0};

  int n;
  std::cout << "Board size: ";
  std::cin >> n;

  int* board = new int[n * n * n];
  resetBoard(board, n);

  *(board) = 1;
  *(board + 1) = 2;

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ConnectX 3D");

  SetTargetFPS(60);

  Camera3D* camera = new Camera3D();
  camera->position = (Vector3) {30.0f, 20.0f, 30.0f};
  camera->target = (Vector3){0.0f, 0.0f, 0.0f};
  camera->up = (Vector3){0.0f, 1.0f, 0.0f};
  camera->fovy = 60.0f;
  camera->projection = CAMERA_PERSPECTIVE;

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
      std::cout << getNextHighestBlock(board, n, sel.x, sel.y) << std::endl;
    }

    //UpdateCamera(camera, CAMERA_ORBITAL);

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

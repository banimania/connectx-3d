#include <raylib.h>
#include <rlgl.h>
#include <iostream>
#include "../include/config.hpp"

void resetBoard(int* board, int n) {
  for (int i = 0; i < n * n * n; i++) {
    board[i] = 0;
  }
}

void renderBoard(int* board, int n) {
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

void handleKeyboard(Camera3D* camera) {
  if (IsKeyDown(KEY_SPACE)) {
    std::cout << "Space" << std::endl;
  }
}

int main() {
  std::cout << "ConnectX 3D by danimania" << std::endl << std::endl;

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
    handleKeyboard(camera);

    UpdateCamera(camera, CAMERA_ORBITAL);

    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode3D(*camera);

    renderBoard(board, n);
    
    EndMode3D();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

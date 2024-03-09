#include <raylib.h>
#include <rlgl.h>
#include <iostream>
#include <string>
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

void renderBoard(int* board, int n, Vector2 sel, float s) {
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
        if (v == 1 || v == 2) {
          Vector3 pos = {s / 2.0f + i * s, s / 2.0f + j * s, s / 2.0f + k * s};
          DrawCube(pos, s, s, s, (v == 1 ? Color{255, 0, 0, 45} : Color{0, 0, 255, 45}));
          DrawCubeWires(pos, s, s, s, (v == 1 ? Color{255, 0, 0, 255} : Color{0, 0, 255, 255}));
        }
      }
    }
  }

  Vector3 pos = {sel.x * s + s / 2.0f, s / 2.0f + getNextHighestBlock(board, n, sel.x, sel.y) * s, sel.y * s + s / 2.0f};
  DrawCube(pos, s, s, s, {255, 255, 255, 55});
  DrawCubeWires(pos, s, s, s, {255, 255, 255, 255});
}

int main() {
  std::cout << "ConnectX 3D by danimania" << std::endl << std::endl;

  Vector2 sel = {0, 0};

  float s = 5.0f;

  int n;
  std::cout << "Board size: ";
  std::cin >> n;

  int* board = new int[n * n * n];
  resetBoard(board, n);

  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ConnectX 3D");

  SetTargetFPS(60);

  float f = 0.2f;

  Camera3D* camera = new Camera3D();
  camera->position = (Vector3) {40.0f * n * f, 15.0f * n * f * 1.5f, 40.0f * n * f};
  camera->target = (Vector3){0.0f, 0.0f, 0.0f};
  camera->up = (Vector3){0.0f, 1.0f, 0.0f};
  camera->fovy = 60.0f;
  camera->projection = CAMERA_PERSPECTIVE;

  int p = 1;
  while(!WindowShouldClose()) {
    //Handle Input
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

    //UpdateCamera(camera, CAMERA_PERSPECTIVE);

    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode3D(*camera);

    renderBoard(board, n, sel, s);

    EndMode3D();

    std::string moveString = "Player " + std::to_string(p) + " moves";
    DrawText(moveString.c_str(), SCREEN_WIDTH / 2.0f - MeasureText(moveString.c_str(), 30.0f) / 2.0f, SCREEN_HEIGHT - 50.0f, 30.0f, {255, 255, 255, 255});
    
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

// Kube - A puzzle video game.
// Copyright (C) 2021 Kendal Harland GPL-3.0-only
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <iostream>
#include <raylib.h>

#include "cube.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define TITLE "Kube"
#define TARGET_FPS 60

#define TILE_SIZE 1.f
#define CUBE_SIZE 1.f

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  SetTargetFPS(TARGET_FPS);

  Camera camera = {0};
  camera.position = (Vector3){10.0f, 10.0f, 10.0f}; // position
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};      // looking at point
  camera.up = (Vector3){0.0f, 2.0f, 0.0f}; // up vector (rot towards target)
  camera.fovy = 45.0f;                     // field-of-view Y
  camera.type = CAMERA_PERSPECTIVE;        // mode type

  Ray ray = {0};

  Vector3 origin = camera.target;
  origin.y += CUBE_SIZE / 2.f;
  kube::Cube cube(origin, CUBE_SIZE, WHITE);

  SetCameraMode(camera, CAMERA_FREE);
  // Detect window close button or ESC key
  while (!WindowShouldClose()) {
    UpdateCamera(&camera);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      ray = GetMouseRay(GetMousePosition(), camera);
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(camera);

    cube.Draw();
    DrawRay(ray, MAROON);
    DrawGrid(10, TILE_SIZE);

    EndMode3D();
    EndDrawing();
  }

  CloseWindow(); // Close window and OpenGL context

  return 0;
}
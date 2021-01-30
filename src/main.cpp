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

#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define TITLE "Kube"
#define TARGET_FPS 60

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  SetTargetFPS(TARGET_FPS);

  // Detect window close button or ESC key
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawText("some basic shapes available on raylib", 20, 20, 20, DARKGRAY);

    DrawCircle(SCREEN_WIDTH / 4, 120, 35, DARKBLUE);

    DrawRectangle(SCREEN_WIDTH / 4 * 2 - 60, 100, 120, 60, RED);
    DrawRectangleLines(SCREEN_WIDTH / 4 * 2 - 40, 320, 80, 60,
                       ORANGE); // NOTE: Uses QUADS internally, not lines
    DrawRectangleGradientH(SCREEN_WIDTH / 4 * 2 - 90, 170, 180, 130, MAROON,
                           GOLD);

    DrawTriangle((Vector2){SCREEN_WIDTH / 4 * 3, 80},
                 (Vector2){SCREEN_WIDTH / 4 * 3 - 60, 150},
                 (Vector2){SCREEN_WIDTH / 4 * 3 + 60, 150}, VIOLET);

    DrawPoly((Vector2){SCREEN_WIDTH / 4 * 3, 320}, 6, 80, 0, BROWN);

    DrawCircleGradient(SCREEN_WIDTH / 4, 220, 60, GREEN, SKYBLUE);

    // NOTE: We draw all LINES based shapes together to optimize internal
    // drawing, this way, all LINES are rendered in a single draw pass
    DrawLine(18, 42, SCREEN_WIDTH - 18, 42, BLACK);
    DrawCircleLines(SCREEN_WIDTH / 4, 340, 80, DARKBLUE);
    DrawTriangleLines((Vector2){SCREEN_WIDTH / 4 * 3, 160},
                      (Vector2){SCREEN_WIDTH / 4 * 3 - 20, 230},
                      (Vector2){SCREEN_WIDTH / 4 * 3 + 20, 230}, DARKBLUE);
    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
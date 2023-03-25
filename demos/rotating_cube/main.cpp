// Kube  :  An OpenGL based game engine.
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


#include <iostream> // For debugging.
#include <stdio.h>
#include <stdlib.h>

#include <kube/camera.h>
#include <kube/graphics.h>
#include <kube/logging.h>
#include <kube/model.h>
#include <kube/player.h>
#include <kube/shapes.cpp>
#include <kube/time.h>
#include <kube/window.h>

#define TITLE "Rotating Cube"

// Camera is at (4,3,-3), in World Space
// and looks at the origin

#define CAMERA_POS glm::vec3(-15, 10, -25)
#define CAMERA_TARGET glm::vec3()

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800


#define ORIGIN glm::vec3(0.0f)

int main(void) {
  kube::Camera camera(CAMERA_POS, CAMERA_TARGET);
  auto window = kube::Window::GetInstance();
  window->Open(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  window->SetCamera(std::move(camera));

  auto model = kube::Cube();
  auto state = kube::PlayerIdleState();
  kube::Player player(&model, &state);

  kube::Stopwatch stopwatch;
  stopwatch.Start();

  do {
    player.HandleInput(window);
    player.Update(stopwatch.Lap());
    window->Clear();
    model.Draw(window);
    window->Update();
  } // Check if the ESC key was pressed or the window was closed
  while (!window->ShouldClose());

  window->Close();

  return 0;
}

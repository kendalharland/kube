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

#include <stdio.h>
#include <stdlib.h>

#include <kube/camera.h>
#include <kube/logging.h>
#include <kube/mesh.h>
#include <kube/model.h>
#include <kube/shapes.cpp>
#include <kube/time.h>
#include <kube/window.h>

#include "controls.hpp"

#define TITLE "Creatures"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

int main(void) {
  using namespace kube::graphics;

  kube::Camera camera;
  auto window = kube::Window::GetInstance();
  window->Open(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  window->SetCamera(&camera);

  // clang-format off
  // TODO: Load assets from file.
  std::string assets[] = {
    "demos/creatures/assets/creatures/crocsoldier/Crocsoldier.obj",
    "demos/creatures/assets/creatures/lizardbomb/lizardbomb-0.obj",
    "demos/creatures/assets/creatures/lizardbomb/lizardbomb-1.obj",
    "demos/creatures/assets/creatures/lizardbomb/lizardbomb-2.obj",
    "demos/creatures/assets/creatures/lizardbomb/lizardbomb-3.obj",
    "demos/creatures/assets/creatures/lizardmonitor/lizardmonitor-0.obj",
    "demos/creatures/assets/creatures/lizardmonitor/lizardmonitor-1.obj",
    "demos/creatures/assets/creatures/snakegod/snakegod-0.obj",
    "demos/creatures/assets/creatures/snakeking/snakeking-0.obj",
    "demos/creatures/assets/creatures/snakeking/snakeking-1.obj",
    "demos/creatures/assets/creatures/snakeking/snakeking-2.obj",
    "demos/creatures/assets/creatures/snakeking/snakeking-3.obj",
    "demos/creatures/assets/creatures/snakewizard/Snakewizard.obj",
  };
  // clang-format on

  shader_ptr shader = Shader::DiffuseShader("src/shaders");
  auto light_position = glm::vec4(1, 1, 1, 1);

  std::vector<std::shared_ptr<kube::Model>> models;
  for (auto asset : assets) {
    auto model = kube::Model::LoadFromFile(asset);
    models.push_back(std::move(model));
  }

  int active_model = 0;

  kube::Actor actor;
  actor.SetModel(models.at(active_model));
  actor.SetState(std::make_unique<IdleState>());

  kube::Stopwatch stopwatch;
  stopwatch.Start();

  float x = 0;
  do {
    auto dt = stopwatch.Lap();

    // Add an orbiting light source to show off the shader.
    x += dt;
    light_position.y = sin(x) * 2;
    light_position.z = sin(x) * 2;
    shader->SetUniformVec4("light_position", light_position);

    // Press space to rotate through models.
    if (window->IsKeyPressed(GLFW_KEY_SPACE)) {
      active_model = (active_model + 1) % models.size();
      actor.SetModel(models.at(active_model));
    }

    actor.HandleInput(window);
    actor.Update(dt);
    window->Clear();
    actor.GetModel()->Draw(&camera, shader);
    window->Update();

  } while (!window->ShouldClose());

  window->Close();

  return 0;
}

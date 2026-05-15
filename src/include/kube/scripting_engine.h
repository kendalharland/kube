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

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <kube/actor.h>
#include <kube/camera.h>
#include <kube/entity.h>
#include <kube/logging.h>
#include <kube/math.h>
#include <kube/mesh.h>
#include <kube/model.h>
#include <kube/scripting_engine.h>
#include <kube/shapes.cpp>
#include <kube/time.h>
#include <kube/window.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#define MAX_MODELS MAX_ENTITIES

namespace kube {

bool streq(std::string a, std::string b) { return a.compare(b) == 0; }

void openWindow(int width, int height, char *title) {
  auto window = kube::Window::GetInstance();
  window->Open(width, height, title);
  auto camera = std::make_unique<kube::Camera>();
  camera->SetPosition(100, 100, 100);
  window->SetCamera(std::move(camera));
}

typedef struct Game {
  EntityStore *entities;
} Game;

void initGame(Game *game) { game->entities = new EntityStore(); }

void freeGame(Game *game) {
  delete game->entities;
  game = nullptr;
}

void runGame(Game *game) {
  using namespace kube::graphics;

  auto window = kube::Window::GetInstance();

  std::shared_ptr<Shader> shader = Shader::SimpleColorShader("src/shaders");

  kube::Stopwatch stopwatch;
  stopwatch.Start();

  do {
    auto deltaSecs = stopwatch.Lap();

    window->Clear();

    // Draw entities
    for (auto entity : game->entities->GetEntitiesWithModelComponent()) {
      auto model = game->entities->GetModelComponent(entity.id);

      // Update position.
      auto position = game->entities->GetPositionComponent(entity.id);
      if (position != nullptr) {
        model->model.SetCenter(position->position);
      }

      // Update rotation.
      auto movement = game->entities->GetMovementComponent(entity.id);
      if (movement != nullptr) {
        auto model = game->entities->GetModelComponent(entity.id);
        model->model.Rotate(movement->spin.x * deltaSecs, X_AXIS);
        model->model.Rotate(movement->spin.y * deltaSecs, Y_AXIS);
        model->model.Rotate(movement->spin.z * deltaSecs, Z_AXIS);
      }

      model->model.Draw(window->GetCamera(), shader);
    }

    window->Update();
  } while (!window->ShouldClose());

  window->Close();
}

// ============================================================================
// Entity
// ============================================================================

static EntityID createEntity(Game *game) { return game->entities->CreateEntity(); }

static void entitySetModel(Game *game, EntityID id, Model &&model) {
  auto component = kube::ModelComponent{};
  component.model = std::move(model);
  game->entities->AddModelComponent(id, std::move(component));
}

static void entitySetPosition(Game *game, EntityID id, glm::vec3 position) {
  auto component = kube::PositionComponent{.position = position};
  game->entities->AddPositionComponent(id, std::move(component));
}

static void entitySetSpin(Game *game, EntityID id, glm::vec3 spin) {
  auto component = kube::MovementComponent{.spin = spin};
  game->entities->AddMovementComponent(id, std::move(component));
}

// ============================================================================
// Model
// ============================================================================

kube::Model loadModelFile(std::string filename) { throw std::logic_error("unimplemented"); }

Model createModel(std::string identifier) {
  if (streq(identifier, "@cube")) {
    return kube::CreateCubeModel();
  } else {
    return loadModelFile(identifier);
  }
}

} // namespace kube
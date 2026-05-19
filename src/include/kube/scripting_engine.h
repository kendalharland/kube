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

#include <kube/camera.h>
#include <kube/entity.h>
#include <kube/graphics.h>
#include <kube/logging.h>
#include <kube/scripting_engine.h>
#include <kube/shapes.h>
#include <kube/time.h>
#include <kube/window.h>

#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>

#define MAX_MODELS MAX_ENTITIES

namespace kube {

bool streq(std::string a, std::string b) { return a.compare(b) == 0; }

void openWindow(int width, int height, char *title) {
  auto window = Window::GetInstance();
  window->Open(width, height, title);
}

typedef struct Game {
  bool running = false;
  EntityStore *entities;
  CameraStore *cameras;
} Game;

Game *newGame() {
  auto game = new Game();
  game->entities = new EntityStore();
  game->cameras = new CameraStore();
  return game;
}

void freeGame(Game *game) {
  delete game->entities;
  delete game->cameras;
  game = nullptr;
}

void gameLoop(Game *game, std::function<void(double)> update) {
  auto window = Window::GetInstance();

  Stopwatch stopwatch;
  stopwatch.Start();

  do {
    auto deltaSecs = stopwatch.ElapsedSeconds();

    update(deltaSecs);

    window->Clear();

    // Draw entities
    for (auto entity : *game->entities) {
      auto model = game->entities->GetComponent<ModelComponent>(entity.id);
      auto position = game->entities->GetComponent<PositionComponent>(entity.id);
      auto movement = game->entities->GetComponent<MovementComponent>(entity.id);

      // Skip entities that have no location in space
      if (position == nullptr) {
        continue;
      }

      // Process entity movement
      if (movement != nullptr) {
        position->position += position->position * movement->velocity;
        position->rotation = glm::rotate(position->rotation, float(deltaSecs), movement->spin);
      }

      // Update camera position
      auto camera = game->cameras->Get(entity.id);
      if (camera != nullptr) {
        cameraSetPosition(camera, position->position);
      }

      // Update and draw model
      if (model != nullptr) {
        auto graphics = game->entities->GetComponent<GraphicsComponent>(entity.id);
        model->model.center = position->position;
        model->model.rotation = position->rotation;
        auto shader = graphics->shader.get();

        // Hack: draw
        // kube::camera camera = Window->GetCamera();
        kube::draw(*window, *window->GetCamera(), model->model, *shader);
        // model->model.Draw(window->GetCamera(), *shader);
      }
    }

    window->Update();

  } while (!window->ShouldClose());

  window->Close();
}

CameraID createCamera(Game *game) {
  auto cameraID = game->entities->Create();
  game->cameras->Create(cameraID);
  return cameraID;
}

static void cameraSetActive(Game *game, CameraID cameraID) {
  auto camera = game->cameras->Get(cameraID);
  if (camera == nullptr) {
    return;
  }
  auto window = Window::GetInstance();
  window->SetCamera(camera);
}

// ============================================================================
// Entity
// ============================================================================

static EntityID createEntity(Game *game) { return game->entities->Create(); }

static void entitySetModel(Game *game, EntityID id, model &&model) {
  auto component = ModelComponent{};
  component.model = std::move(model);
  game->entities->SetComponent(id, std::move(component));
}

static void entitySetPosition(Game *game, EntityID id, glm::vec3 position) {
  auto component = PositionComponent{.position = position};
  auto previous = game->entities->GetComponent<PositionComponent>(id);
  if (previous != nullptr) {
    component.rotation = previous->rotation;
  }
  game->entities->SetComponent(id, std::move(component));
}

static void entitySetSpin(Game *game, EntityID id, glm::vec3 spin) {
  auto component = MovementComponent{.spin = spin};
  game->entities->SetComponent(id, std::move(component));
}

void entitySetShader(Game *game, EntityID id, std::shared_ptr<kube::shader> shader) {
  auto component = GraphicsComponent{.shader = shader};
  game->entities->SetComponent(id, std::move(component));
}

// ============================================================================
// model
// ============================================================================

mesh createMesh(std::string identifier) {
  if (streq(identifier, "@cube")) {
    return cubeMesh();
  } else if (streq(identifier, "@quad")) {
    return quadMesh();
  } else {
    throw std::invalid_argument("invalid mesh identifier");
  }
}

model createModel(std::string identifier) {
  model model;
  if (identifier[0] == '@') {
    model.meshes.push_back(createMesh(identifier));
  } else {
    model = loadModelFromFile(identifier);
  }

  for (auto &mesh : model.meshes) {
    meshLoad(mesh);
  }

  return model;
}

// ============================================================================
// shader
// ============================================================================

shader loadShader(std::string path) {
  shader shader = shaderLoadFrom(std::filesystem::path(path));
  shaderLoad(shader);
  // TODO: Cache so we don't have to keep recompiling.
  return shader;
}

} // namespace kube
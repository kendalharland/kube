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

auto entities = kube::EntityStore();

namespace kube {

bool streq(std::string a, std::string b) { return a.compare(b) == 0; }

void openWindow(int width, int height, char *title) {
  auto window = kube::Window::GetInstance();
  window->Open(width, height, title);
  auto camera = std::make_unique<kube::Camera>();
  camera->SetPosition(100, 100, 100);
  window->SetCamera(std::move(camera));
}

// ============================================================================
// Entity
// ============================================================================

static void entitySetModel(EntityID id, Model&& model) {
  auto component = kube::ModelComponent{};
  component.model = std::move(model);
  entities.AddModelComponent(id, std::move(component));
}

static void entitySetPosition(EntityID id, glm::vec3 position) {
  auto component = kube::PositionComponent{.position = position};
  entities.AddPositionComponent(id, std::move(component));
}

static void entitySetSpin(EntityID id, glm::vec3 spin) {
  auto component = kube::MovementComponent{.spin = spin};
  entities.AddMovementComponent(id, std::move(component));
}

// ============================================================================
// Model
// ============================================================================

kube::Model loadModelFile(std::string filename) {
    throw std::logic_error("unimplemented");
}

Model createModel(std::string identifier) {
    if (streq(identifier, "@cube")) {
        return kube::CreateCubeModel();
    } else {
        return loadModelFile(identifier);
    }
}

} // namespace kube
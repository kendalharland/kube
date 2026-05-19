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

#include <glm/glm.hpp>
#include <map>
#include <numbers>

#define GLM_FORCE_RADIANS 1

#define CameraID int

namespace kube {

typedef struct camera {
  float aspectRatio = 4.0f / 3.0f;
  float far = 100.f;
  float fov = glm::radians(45.0f);
  float near = 0.1f;
  float zoomSpeed = 0.05f;
  glm::vec3 position = glm::vec3(10.0f);
  glm::vec3 target = glm::vec3(0.f);
  glm::vec3 up = glm::vec3(0, 1, 0);
} camera;

void cameraSetPosition(camera *camera, glm::vec3 position) { camera->position = position; }

void cameraZoom(camera &camera, bool in) {
  camera.fov += in ? -camera.zoomSpeed : camera.zoomSpeed;
  camera.fov = std::min(camera.fov, (float)std::numbers::pi);
  camera.fov = std::max(camera.fov, (float)M_2_PI);
}

class CameraStore {
public:
  CameraStore();
  ~CameraStore();

  void Create(CameraID cameraID);
  camera *Get(CameraID cameraID);

private:
  std::map<CameraID, camera> cameras_;
};

CameraStore::CameraStore() {}

CameraStore::~CameraStore() {}

void CameraStore::Create(CameraID id) { cameras_.emplace(id, camera()); }

camera *CameraStore::Get(CameraID id) {
  auto result = cameras_.find(id);
  if (result == cameras_.end()) {
    return nullptr;
  }
  return &result->second;
}

}; // namespace kube

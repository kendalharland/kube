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
#include <glm/gtc/matrix_transform.hpp>

#define GLM_FORCE_RADIANS 1

namespace kube {

class Camera final {
public:
  Camera() {}
  Camera(const Camera &camera) = delete;
  Camera(Camera &&other) = default;
  Camera &operator=(Camera &&other) = default;

  void SetAspectRatio(float value);
  float GetAspsectRatio() const;
  void SetPosition(float x, float y, float z);
  glm::vec3 GetPosition() const;
  void SetTarget(float x, float y, float z);
  glm::vec3 GetTarget() const;

  glm::mat4 ComputeMVP(const glm::mat4 model) const;
  void Zoom(bool in);

private:
  float aspectRatio_ = 4.0f / 3.0f;
  float far_ = 100.f;
  float fov_ = glm::radians(45.0f);
  float near_ = 0.1f;
  float zoomSpeed_ = 0.05f;
  glm::vec3 position_ = glm::vec3(10.0f);
  glm::vec3 target_ = glm::vec3(0.f);
  glm::vec3 up_ = glm::vec3(0, 1, 0);
};

}; // namespace kube

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

#include <algorithm>

#include <kube/camera.h>

#include <glm/glm.hpp>

namespace kube {

void Camera::SetAspectRatio(float value) { aspectRatio_ = value; }

float Camera::GetAspsectRatio() const { return aspectRatio_; }

void Camera::SetPosition(float x, float y, float z) { position_ = glm::vec3(x, y, x); }

glm::vec3 Camera::GetPosition() const { return position_; }

void Camera::SetTarget(float x, float y, float z) { target_ = glm::vec3(x, y, z); }

glm::vec3 Camera::GetTarget() const { return target_; }

glm::mat4 Camera::ComputeMVP(const glm::mat4 model) const {
  auto projection = glm::perspective(fov_, aspectRatio_, near_, far_);
  auto view = glm::lookAt(position_, target_, up_);
  return projection * view * model;
}

void Camera::Zoom(bool in) {
  fov_ += in ? -zoomSpeed_ : zoomSpeed_;
  fov_ = std::min(fov_, (float)M_PI_4f32);
  fov_ = std::max(fov_, (float)M_2_PI);
}

} // namespace kube

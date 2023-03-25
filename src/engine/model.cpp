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

#include <kube/model.h>
#include <kube/window.h>

#define IDENTITY_MAT4 glm::mat4(1.f)

namespace kube {

// TODO: Delete.
const glm::vec3 Model::Center() { return center_; }
const glm::mat4 Model::Rotation() { return rotation_; }
const glm::mat4 Model::Scale() { return scale_; }

void Model::Draw(Window *window) {
  glm::mat4 translation = glm::translate(IDENTITY_MAT4, Center());
  glm::mat4 rotation = Rotation();
  glm::mat4 scale = Scale();
  glm::mat4 mvp = window->GetCamera().MVP(translation * rotation * scale);
  mesh_.Draw(shader_, mvp);
}

void Model::SetRotation(float radians, glm::vec3 axis) {
  rotation_ = glm::rotate(IDENTITY_MAT4, radians, axis);
}

void Model::RotateRight(float t) {
  auto angle = glm::radians(3.0f) * t;
  rotation_ = glm::rotate(rotation_, angle, -Y_AXIS);
}

void Model::RotateLeft(float t) {
  auto angle = glm::radians(3.0f) * t;
  rotation_ = glm::rotate(rotation_, angle, Y_AXIS);
}

}; // namespace kube

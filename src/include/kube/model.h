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

#ifndef _MODEL_H
#define _MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <kube/graphics.h>
#include <kube/window.h>

static const glm::vec3 Y_AXIS = glm::vec3(0.f, 1.f, 0.f);
static const glm::vec3 X_AXIS = glm::vec3(1.f, 0.f, 0.f);
static const glm::vec3 Z_AXIS = glm::vec3(0.f, 0.f, 1.f);

namespace kube {

class Model {
public:
  Model(graphics::Mesh &&mesh, Shader &&shader) : mesh_(std::move(mesh)), shader_(shader) {
    mesh_.Load();
    shader_.Load();
  }

  ~Model() {
    mesh_.Unload();
    shader_.Unload();
  }

  // TODO: Should we use a unique ptr?
  Model(const Model &model) = delete;
  Model(Model &&other) = default;
  Model &operator=(Model &&other) = default;

  graphics::Mesh mesh_;
  Shader shader_;

  const glm::vec3 Center();
  const glm::mat4 Rotation();
  const glm::mat4 Scale();

  void Draw(Window *window);
  void SetRotation(float radians, glm::vec3 axis);

private:
  // TODO: Move to Geometry class?
  glm::vec3 center_ = glm::vec3(0.0f);
  glm::mat4 scale_ = glm::mat4(2.f);
  glm::mat4 rotation_ = glm::mat4(1.f);
};

} // namespace kube

#endif // _MODEL_H
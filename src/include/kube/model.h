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

#include <memory>

#include <kube/camera.h>
#include <kube/mesh.h>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace kube {

class Model final {

public:
  static std::shared_ptr<Model> LoadFromFile(std::string filename);

  Model();
  Model(std::unique_ptr<graphics::Mesh> mesh);

  Model &operator=(Model &&other) = delete;
  std::string GetName() const;
  glm::mat4 GetMatrix() const;
  void SetName(std::string name);
  void SetCenter(glm::vec3 center);
  void SetRotation(glm::mat4 rotation);
  void SetScale(glm::mat4 scale);
  void AddMesh(std::unique_ptr<graphics::Mesh> mesh);
  void DebugPrint() const;
  void Draw(const Camera *camera, std::shared_ptr<graphics::Shader> shader);
  void Rotate(float radians, glm::vec3 axis);

private:
  Model(const Model &model) = delete;
  Model(Model &&other) = delete;

  std::string name_ = "";
  std::vector<std::unique_ptr<graphics::Mesh>> meshes_;
  glm::vec3 center_ = glm::vec3(0.0f);
  glm::mat4 scale_ = glm::mat4(1.f);
  glm::mat4 rotation_ = glm::mat4(1.f);
};

} // namespace kube

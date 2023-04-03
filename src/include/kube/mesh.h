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

#include <functional>
#include <vector>

#include <kube/shader.h>
#include <kube/texture.h>
#include <kube/vertex_array.h>

#include <kube/internal/constructors.h>

#include <glm/glm.hpp>

namespace kube {
namespace graphics {

struct Material {
  glm::vec3 color_ambient;
  glm::vec3 color_emissive;
  glm::vec3 color_diffuse;
  glm::vec3 color_specular;
  float shininess;
};

class Mesh final {
public:
  UNCOPYABLE(Mesh);
  Mesh(vertex_array_ptr vertices);
  Mesh(vertex_array_ptr vertices, std::vector<texture_ptr> textures);
  Mesh(vertex_array_ptr vertices, std::vector<texture_ptr> textures, Material &&material);
  Mesh() = delete;
  ~Mesh();

  void Draw(std::shared_ptr<Shader> shader) const;
  void DebugPrint() const;

private:
  void Load();
  vertex_array_ptr vertices_;
  std::vector<texture_ptr> textures_;
  Material material_;
};

}; // namespace graphics
}; // namespace kube

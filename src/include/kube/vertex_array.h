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

#include <vector>

#include <kube/internal/constructors.h>

#include <glm/glm.hpp>

namespace kube {
namespace graphics {

struct Vertex {
  glm::vec3 position;
  glm::vec3 colors;
  glm::vec3 normal;
  glm::vec2 tex_coords;
  glm::vec3 tangent;
  glm::vec3 bitangent;
};

class VertexArray final {
public:
  UNCOPYABLE(VertexArray);
  VertexArray() {}
  VertexArray(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
  ~VertexArray() {}

  void Load();
  void Unload();
  void Draw() const;
  void DebugPrint() const;

private:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  unsigned int VAO_; // vertex array id.
  unsigned int VBO_; // vertex buffer id.
  unsigned int EBO_; // element array buffer id for VBO indexing.
};

typedef std::unique_ptr<VertexArray> vertex_array_ptr;

} // namespace graphics
} // namespace kube
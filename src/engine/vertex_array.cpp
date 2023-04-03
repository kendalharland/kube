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

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <kube/logging.h>
#include <kube/vertex_array.h>

namespace kube {
namespace graphics {

VertexArray::VertexArray(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
  vertices_ = vertices;
  indices_ = indices;
}

void VertexArray::Load() {
  KUBE_INFO << "Allocating vertex array";
  glGenVertexArrays(1, &VAO_);
  glBindVertexArray(VAO_);

  glGenBuffers(1, &VBO_);
  glGenBuffers(1, &EBO_);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0],
               GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,              // location = 0 in shader file.
                        3,              // the position has [x, y, z] 3 elements.
                        GL_FLOAT,       // element type.
                        GL_FALSE,       // do not normalize
                        sizeof(Vertex), // stride
                        (void *)0);     // offset
  // vertex colors
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,                                 // location = 1 in shader file
                        3,                                 // the color has [r, g, b] 3 elements.
                        GL_FLOAT,                          // element type.
                        GL_FALSE,                          // do not normalize.
                        sizeof(Vertex),                    // stride
                        (void *)offsetof(Vertex, colors)); // offset.

  // vertex normal
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

  // vertex texture coords
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, tex_coords));

  // vertex tangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, tangent));

  // vertex bitangent
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, bitangent));

  glBindVertexArray(0);
}

void VertexArray::Unload() {
  KUBE_INFO << "Deallocating vertex array";
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
  glDeleteBuffers(1, &EBO_);
}

void VertexArray::Draw() const {
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (void *)0);
  glBindVertexArray(0);
}

void VertexArray::DebugPrint() const {
  // clang-format off
  KUBE_DEBUG << "Mesh:" << std::endl
      << "  NumVertices: " << vertices_.size() << std::endl
      << "  NumIndices:" << indices_.size() << std::endl;
  // clang-format on
}

}; // namespace graphics
}; // namespace kube
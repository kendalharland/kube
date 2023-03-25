// Kube - A puzzle video game.
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
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <logging.h>
#include <opengl.h>

namespace kube {
namespace graphics {

// clang-format off
void Mesh::Load() {
  KUBE_INFO("loading mesh");
  unsigned int VAO; // vertex arrays
  unsigned int VBO; // vertex buffer
  unsigned int EBO; // element array buffer

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO); // OR glEnableVertexAttribArray(VAO);

  // Create and fill buffers with data.
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.GetEBO());
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int),
  // &mesh.indices[0], GL_STATIC_DRAW);

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
  // vertex normals
  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex,
  // normal)); vertex texture coords glEnableVertexAttribArray(2); glVertexAttribPointer(3, 2,
  // GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coords));

  glBindVertexArray(0);

  VAO_ = VAO;
  VBO_ = VBO;
  EBO_ = EBO;
}
// clang-format on

void Mesh::Unload() {
  KUBE_INFO("unloading mesh");
  // TODO: VAO?
  glDeleteBuffers(1, &VBO_);
  glDeleteBuffers(1, &EBO_);
}

void Mesh::Draw(Shader &shader, glm::mat4 mvp /* TODO: Remove mvp */) {
  shader.SetMVP(mvp);
  shader.Use();
  glBindVertexArray(VAO_);
  glDrawArrays(GL_TRIANGLES, 0, indices.size());
  glBindVertexArray(0);
}

}; // namespace graphics
}; // namespace kube
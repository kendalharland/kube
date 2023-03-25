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
#include <mesh.h>
#include <opengl.h>

namespace kube {
namespace graphics {

// clang-format off
void LoadMesh(Mesh &mesh) {
  unsigned int VAO; // vertex arrays
  unsigned int VBO; // vertex buffer
  unsigned int EBO; // element array buffer

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO); // OR glEnableVertexAttribArray(VAO);

  // Create and fill buffers with data.
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.GetEBO());
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,              // location = 0 in shader file.
                        3,              // the position has [x, y, z] 3 elements.
                        GL_FLOAT,       // element type.
                        GL_FALSE,       // do not normalize
                        sizeof(Vertex), // stride
                        (void*)0);      // offset 
  // vertex colors
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,              // location = 1 in shader file
                        3,              // the color has [r, g, b] 3 elements.
                        GL_FLOAT,       // element type.
                        GL_FALSE,       // do not normalize.
                        sizeof(Vertex), // stride
                        (void *)offsetof(Vertex, colors)); // offset.
  // vertex normals
  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  // vertex texture coords
  // glEnableVertexAttribArray(2);
  // glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coords));

  glBindVertexArray(0);
  
  mesh.SetVAO(VAO);
  mesh.SetVBO(VBO);
  mesh.SetEBO(EBO);  
}

void UnloadMesh(Mesh& mesh) {
  unsigned int VBO = mesh.GetVBO();
  unsigned int EBO = mesh.GetEBO();

  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void DrawMesh(Mesh& mesh, Shader& shader, glm::mat4 mvp /* TODO: Remove mvp */) {
  KUBE_INFO("Drawing mesh");

  shader.SetMVP(mvp);
  shader.Use(); // TODO: Flip order?
  glBindVertexArray(mesh.GetVAO());
  glDrawArrays(GL_TRIANGLES, 0, mesh.indices.size());//, 3);//GL_UNSIGNED_BYTE, 0);
  glBindVertexArray(0);
}

// clang-format on

}; // namespace graphics
}; // namespace kube
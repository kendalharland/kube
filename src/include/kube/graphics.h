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

#ifndef _OPENGL_H
#define _OPENGL_H

#include <functional>
#include <vector>

#include <kube/shader.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace kube {
namespace graphics {

class Mesh;
struct Texture;
struct Vertex;

void LoadMesh(Mesh &mesh);
void UnloadMesh(Mesh &mesh);
void DrawMesh(Mesh &mesh, Shader &shader, glm::mat4 mvp);

struct Vertex {
  glm::vec3 position;
  glm::vec3 colors;
  // glm::vec3 normal;
  // glm::vec2 tex_coords;
};

struct Texture {
  unsigned int id;
  std::string type;
};

class Mesh {

public:
  Mesh() = delete;
  Mesh(const Mesh &other) = delete;
  Mesh(Mesh &&other) = default;
  Mesh &operator=(Mesh &&other) = default;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
      : vertices(vertices), indices(indices) {}

  void Load();
  void Unload();
  void Draw(Shader &shader, glm::mat4 mvp); // TODO: Make camera

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

private:
  unsigned int VAO_; // vertex arrays
  unsigned int VBO_; // vertex buffer
  unsigned int EBO_; // element array buffer
};

}; // namespace graphics
}; // namespace kube

#endif // _OPENGL_H
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

#ifndef _VERTEX_H
#define _VERTEX_H

#include <shader.h>

#include <glm/glm.hpp>
#include <vector>

namespace kube {

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coords;
};

struct Texture {
  unsigned int id;
  std::string type;
};

class Mesh {

public:
  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures)
      : vertices(vertices), indices(indices), textures(textures) {}

  void Draw(Shader &shader);
  void SetVAO(unsigned int VAO);
  void SetVBO(unsigned int VBO);
  void SetEBO(unsigned int EBO);

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

private:
  unsigned int VAO_; // vertex arrays
  unsigned int VBO_; // vertex buffer
  unsigned int EBO_; // element array buffer
};

}; // namespace kube

#endif // _VERTEX_H
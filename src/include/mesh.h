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
  Mesh(std::vector<Vertex> vertices,
       std::vector<unsigned int> indices,
       std::vector<Texture> textures)
      : vertices_(vertices), indices_(indices), textures_(textures) {
    init();
  }

  void Draw(Shader& shader);

 private:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<Texture> textures_;

  unsigned int VAO;  // vertex arrays
  unsigned int VBO;  // vertex buffer
  unsigned int EBO;  // element array buffer

  void init();
};

};  // namespace kube

#endif  // _VERTEX_H
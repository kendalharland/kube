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

class MeshBuilder;

struct Vertex {
  glm::vec3 position;
  glm::vec3 colors;
  glm::vec3 normal;
  glm::vec2 tex_coords;
  glm::vec3 tangent;
  glm::vec3 bitangent;
};

struct Texture {
  // OpenGL texture id. Initialized when the texture is loaded.
  unsigned int id;

  // The path to this texture's file.
  std::string filename;
  std::string type;
};

class Mesh {
  friend MeshBuilder;

public:
  Mesh(){};
  Mesh(const Mesh &other) = default;
  Mesh(Mesh &&other) = default;
  Mesh &operator=(Mesh &&other) = default;

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
      : vertices_(vertices), indices_(indices) {}

  void Load();
  void Unload();
  void Draw(Shader *shader);

  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<Texture> textures_;

private:
  unsigned int VAO_; // vertex array id.
  unsigned int VBO_; // vertex buffer id.
  unsigned int EBO_; // element array buffer id for VBO indexing.
};

// TODO: Consider the following:
// - Create ModelBuilder.
// - Create TextureBuilder.
// - Compose these builders.
class MeshBuilder {
public:
  void AddVertex(Vertex &&vertex);
  void AddIndex(unsigned int index);
  void AddTexture(Texture &&texture);
  Mesh Build();

private:
  Mesh mesh_;
};

}; // namespace graphics
}; // namespace kube

#endif // _OPENGL_H
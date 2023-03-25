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

#include <kube/graphics.h>
#include <kube/logging.h>

#define STB_IMAGE_IMPLEMENTATION
#include <kube/internal/stb_image.h>

namespace kube {
namespace graphics {

// TODO: Declare this somewhere

void LoadTexture_(Texture *texture) {
  KUBE_INFO("Loading texture " + texture->filename);

  glGenTextures(1, &texture->id);
  glBindTexture(GL_TEXTURE_2D, texture->id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, num_channels;
  unsigned char *data = stbi_load(texture->filename.c_str(), &width, &height, &num_channels, 0);
  if (data) {
    GLenum format;
    if (num_channels == 1) {
      format = GL_RED;
    } else if (num_channels == 3) {
      format = GL_RGB;
    } else if (num_channels == 4) {
      format = GL_RGBA;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    KUBE_ERROR("Failed to load texture from " + texture->filename);
  }

  stbi_image_free(data);
}

void MeshBuilder::AddVertex(graphics::Vertex &&vertex) {
  mesh_.vertices_.push_back(std::move(vertex));
}

void MeshBuilder::AddIndex(unsigned int index) { mesh_.indices_.push_back(std::move(index)); }

void MeshBuilder::AddTexture(graphics::Texture &&texture) {
  mesh_.textures_.push_back(std::move(texture));
}

Mesh MeshBuilder::Build() { return std::move(mesh_); }

// clang-format off
void Mesh::Load() {
  // KUBE_INFO("loading mesh textures");
  // for (auto texture : textures_) {
  //   LoadTexture_(&texture);
  // }

  KUBE_INFO("Loading mesh vertices");
  unsigned int VAO; // vertex array.
  unsigned int VBO; // vertex buffer.
  unsigned int EBO; // element array buffer for VBO indexing.

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

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
  glVertexAttribPointer(2,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void *)offsetof(Vertex,  normal));

  // vertex texture coords
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void *)offsetof(Vertex,  tex_coords));

  // vertex tangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void *)offsetof(Vertex,  tangent));

  // vertex bitangent
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Vertex),
                        (void *)offsetof(Vertex,  bitangent));

  glBindVertexArray(0);

  VAO_ = VAO;
  VBO_ = VBO;
  EBO_ = EBO;
}
// clang-format on

void Mesh::Unload() {
  KUBE_INFO("Unloading mesh");
  glDeleteVertexArrays(1, &VAO_);
  glDeleteBuffers(1, &VBO_);
  glDeleteBuffers(1, &EBO_);
  // TODO: Textures!
}

void Mesh::Draw(Shader *shader) {
  if (textures_.size() > 0) {
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->GetProgramID(), "sampler"), 0);
    glBindTexture(GL_TEXTURE_2D, textures_[0].id);
  }
  glBindVertexArray(VAO_);
  glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (void *)0);
  glBindVertexArray(0);

  if (textures_.size() > 0) {
    glActiveTexture(GL_TEXTURE0);
  }
}

}; // namespace graphics
}; // namespace kube
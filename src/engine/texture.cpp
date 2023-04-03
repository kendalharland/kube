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
#include <kube/mesh.h>
#include <kube/shader.h>
#include <kube/texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <kube/internal/stb_image.h>

namespace kube {
namespace graphics {

Texture::Texture(std::string filename, std::string type) {
  filename_ = filename;
  type_ = type;
}

std::string Texture::GetFilename() const { return filename_; }

void Texture::Load() {
  KUBE_INFO << "Allocating texture " << filename_;

  glGenTextures(1, &ID_);
  glBindTexture(GL_TEXTURE_2D, ID_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, num_channels;
  unsigned char *data = stbi_load(filename_.c_str(), &width, &height, &num_channels, 0);
  if (data) {
    GLenum format = GL_RGB;
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
    KUBE_ERROR << "Failed to load texture from " << filename_;
  }

  stbi_image_free(data);
}

void Texture::Unload() {
  KUBE_INFO << "Deallocating texture " << ID_;
  glDeleteTextures(1, &ID_);
}

void Texture::Bind(std::shared_ptr<Shader> shader, unsigned int index) const {
  auto location = glGetUniformLocation(shader->GetProgramID(), type_.c_str());
  glActiveTexture(GL_TEXTURE0 + index);
  glUniform1i(location, index);
  glBindTexture(GL_TEXTURE_2D, ID_);
}

}; // namespace graphics
}; // namespace kube
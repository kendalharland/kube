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
#include <kube/texture.h>
#include <kube/vertex_array.h>

namespace kube {
namespace graphics {

Mesh::Mesh(vertex_array_ptr vertices) {
  vertices_ = std::move(vertices);
  Load();
}

Mesh::Mesh(vertex_array_ptr vertices, std::vector<texture_ptr> textures) {
  vertices_ = std::move(vertices);
  textures_ = std::move(textures);
  Load();
}

Mesh::Mesh(vertex_array_ptr vertices, std::vector<texture_ptr> textures, Material &&material) {
  vertices_ = std::move(vertices);
  textures_ = std::move(textures);
  material_ = std::move(material);
  Load();
}

void Mesh::Load() {
  vertices_->Load();
  // for (const texture : textures_) {
  for (int i = 0; i < textures_.size(); i++) {
    textures_[i]->Load();
  }
}

Mesh::~Mesh() {
  vertices_->Unload();
  for (int i = 0; i < textures_.size(); i++) {
    textures_[i]->Unload();
  }
}

void Mesh::DebugPrint() const {
  vertices_->DebugPrint();
  // clang-format off
  KUBE_DEBUG << "  NumTextures: " << textures_.size() << std::endl
      << "  Material: " << std::endl
      << "    ColorAmbient: " << material_.color_ambient.r << ", " << material_.color_ambient.g << ", " << material_.color_ambient.b << ")" << std::endl
      << "    ColorEmissive: " << material_.color_emissive.r << ", " << material_.color_emissive.g << ", " << material_.color_emissive.b << ")" << std::endl
      << "    ColorDiffuse: " << material_.color_diffuse.r << ", " << material_.color_diffuse.g << ", " << material_.color_diffuse.b << ")" << std::endl
      << "    ColorSpecular: " << material_.color_specular.r << ", " << material_.color_specular.g << ", " << material_.color_specular.b << ")" << std::endl;
  // clang-format on
}

void Mesh::Draw(std::shared_ptr<Shader> shader) const {
  shader->SetUniformVec4("light_color", glm::vec4(material_.color_diffuse, 1));
  for (int i = 0; i < textures_.size(); i++) {
    textures_[i]->Bind(shader, i);
  }
  vertices_->Draw();
  if (textures_.size() > 0) {
    glActiveTexture(GL_TEXTURE0);
  }
}

}; // namespace graphics
}; // namespace kube
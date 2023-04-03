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

#include <kube/logging.h>
#include <kube/mesh.h>
#include <kube/model.h>
#include <kube/window.h>

#include "math.h"
#include "model_loader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace kube {

std::shared_ptr<Model> Model::LoadFromFile(std::string filename) {
  return ModelLoader().LoadFromFile(filename);
}

Model::Model() {}

Model::Model(std::unique_ptr<graphics::Mesh> mesh) { meshes_.push_back(std::move(mesh)); }

void Model::DebugPrint() const {
  KUBE_DEBUG << "Model: " << name_ << std::endl
             << "  NumMeshes: " << meshes_.size() << std::endl
             << "  Center: (" << center_.x << ", " << center_.x << ", " << center_.x << ")"
             << std::endl;
}

std::string Model::GetName() const { return name_; }

glm::mat4 Model::GetMatrix() const {
  auto translation = glm::translate(IDENTITY_MAT4, center_);
  return translation * rotation_ * scale_;
}

void Model::AddMesh(std::unique_ptr<graphics::Mesh> mesh) { meshes_.push_back(std::move(mesh)); }
void Model::SetName(std::string name) { name_ = std::move(name); }
void Model::SetCenter(glm::vec3 center) { center_ = std::move(center); }
void Model::SetRotation(glm::mat4 rotation) { rotation_ = std::move(rotation); }
void Model::SetScale(glm::mat4 scale) { scale_ = std::move(scale); }

// Consider doing the following here:
//   shader.SetFloat("center", model->GetCenter());
//   shader.SetFloat("rotation", model->GetRotation());
//   shader.SetFloat("scale", model->GetScale());
// And the following in the caller:
//   shader.SetFloat("perspective", camera->Perspective());
//   shader.SetFloat("view", camera->View())
// And then we can get rid of `window` and just call shader.Use().
void Model::Draw(const Camera *camera, std::shared_ptr<kube::graphics::Shader> shader) {
  shader->Use();
  shader->SetUniformMat4("MVP", camera->ComputeMVP(GetMatrix()));
  for (int i = 0; i < meshes_.size(); i++) {
    meshes_[i]->Draw(shader);
  }
}

void Model::Rotate(float radians, const glm::vec3 axis) {
  rotation_ = glm::rotate(rotation_, radians, axis);
}
}; // namespace kube

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

#pragma once

#include <memory>

#include <kube/mesh.h>
#include <kube/model.h>

#include "mesh_builder.h"

#include <assimp/scene.h>

using namespace kube;

class ModelLoader {
public:
  std::shared_ptr<Model> LoadFromFile(std::string filename);

private:
  void LoadScene_(const aiScene *scene, Model *model);
  void LoadNode_(aiNode *node, const aiScene *scene, Model *model);
  void LoadMesh_(aiMesh *m, const aiScene *scene, MeshBuilder *mesh);
  void LoadVertex_(aiMesh *m, unsigned int i, graphics::Vertex *vertex);
  void LoadMaterial_(aiMaterial *mat, const aiScene *scene, graphics::Material *material);
  void LoadMaterialTextures_(aiMaterial *mat, aiTextureType type, std::string type_name,
                             MeshBuilder *mesh);
  std::string directory_;
};

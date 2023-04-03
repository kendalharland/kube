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
#include "mesh_builder.h"
#include "model_loader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

void StringReplace(std::string *input, std::string old, std::string rep) {
  size_t index = 0;
  while (true) {
    index = input->find(old, index);
    if (index == std::string::npos)
      break;
    input->replace(index, old.length(), rep);
    index += old.length();
  }
}

std::shared_ptr<Model> ModelLoader::LoadFromFile(std::string filename) {
  KUBE_INFO << "Loading scene from file " << filename;

  directory_ = filename.substr(0, filename.find_last_of('/'));
  Assimp::Importer importer;
  auto options = aiProcess_Triangulate | aiProcess_FlipUVs;
  auto scene = importer.ReadFile(filename, options);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    KUBE_ERROR << "ASSIMP::" + std::string(importer.GetErrorString());
    throw std::runtime_error("failed to load model");
  }

  auto model = new Model();
  LoadScene_(scene, model);
  model->SetCenter(glm::vec3(0, 0, 0));
  model->SetScale(glm::mat4(1.f));
  model->SetRotation(glm::mat4(1.f));
  model->DebugPrint();

  return std::shared_ptr<Model>(model);
}

void ModelLoader::LoadScene_(const aiScene *scene, Model *model) {
  LoadNode_(scene->mRootNode, scene, model);
}

void ModelLoader::LoadNode_(aiNode *node, const aiScene *scene, Model *model) {
  model->SetName(node->mName.C_Str());
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    MeshBuilder mesh;
    LoadMesh_(scene->mMeshes[node->mMeshes[i]], scene, &mesh);
    model->AddMesh(mesh.Build());
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    LoadNode_(node->mChildren[i], scene, model);
  }
}

void ModelLoader::LoadVertex_(aiMesh *m, unsigned int i, graphics::Vertex *vertex) {
  glm::vec3 position;
  position.x = m->mVertices[i].x;
  position.y = m->mVertices[i].y;
  position.z = m->mVertices[i].z;
  vertex->position = std::move(position);

  if (m->HasNormals()) {
    glm::vec3 normal;
    normal.x = m->mNormals[i].x;
    normal.y = m->mNormals[i].y;
    normal.z = m->mNormals[i].z;
    vertex->normal = std::move(normal);
  }

  if (m->HasTextureCoords(0)) { // does the mesh contain texture coordinates?
    glm::vec2 tex_coords;
    // A vertex can have 8 different texture coordinates. Assume we're only using models with a
    // single texture coordinate, and just take the first set (0).
    tex_coords.x = m->mTextureCoords[0][i].x;
    tex_coords.y = m->mTextureCoords[0][i].y;
    vertex->tex_coords = std::move(tex_coords);
  } else {
    vertex->tex_coords = std::move(glm::vec2(0.0f, 0.0f));
  }

  if (m->HasTangentsAndBitangents()) {
    glm::vec3 tangent;
    tangent.x = m->mTangents[i].x;
    tangent.y = m->mTangents[i].y;
    tangent.z = m->mTangents[i].z;
    vertex->tangent = std::move(tangent);

    glm::vec3 bitangent;
    bitangent.x = m->mBitangents[i].x;
    bitangent.y = m->mBitangents[i].y;
    bitangent.z = m->mBitangents[i].z;
    vertex->bitangent = std::move(bitangent);
  } else {
    vertex->tangent = std::move(glm::vec3(0.0f));
    vertex->bitangent = std::move(glm::vec3(0.0f));
  }
}

void ModelLoader::LoadMaterialTextures_(aiMaterial *mat, aiTextureType type, std::string type_name,
                                        MeshBuilder *mesh) {
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString texture_path;
    mat->GetTexture(type, i, &texture_path);
    // TODO: Some textures are loaded twice - the filenames match. Skip those ones.
    auto filename = directory_ + '/' + texture_path.C_Str();
    StringReplace(&filename, "\\", "/");
    auto texture = std::make_unique<Texture>(filename, type_name);
    mesh->AddTexture(std::move(texture));
  }
}

void ModelLoader::LoadMaterial_(aiMaterial *m, const aiScene *scene, graphics::Material *material) {
  aiColor3D ambient(0.f, 0.f, 0.f);
  aiColor3D diffuse(0.f, 0.f, 0.f);
  aiColor3D emissive(0.f, 0.f, 0.f);
  aiColor3D specular(0.f, 0.f, 0.f);

  if (AI_SUCCESS != m->Get(AI_MATKEY_COLOR_AMBIENT, ambient)) {
    throw std::runtime_error("failed to get AI_MATKEY_COLOR_AMBIENT");
  }
  if (AI_SUCCESS != m->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse)) {
    throw std::runtime_error("failed to get AI_MATKEY_COLOR_DIFFUSE");
  }
  if (AI_SUCCESS != m->Get(AI_MATKEY_COLOR_EMISSIVE, emissive)) {
    throw std::runtime_error("failed to get AI_MATKEY_COLOR_EMISSIVE");
  }
  if (AI_SUCCESS != m->Get(AI_MATKEY_COLOR_SPECULAR, specular)) {
    throw std::runtime_error("failed to get AI_MATKEY_COLOR_SPECULAR");
  }

  material->color_ambient = glm::vec3((float)ambient.r, (float)ambient.g, (float)ambient.b);
  material->color_diffuse = glm::vec3((float)diffuse.r, (float)diffuse.g, (float)diffuse.b);
  material->color_emissive = glm::vec3((float)emissive.r, (float)emissive.g, (float)emissive.b);
  material->color_specular = glm::vec3((float)specular.r, (float)specular.g, (float)specular.b);
}

void ModelLoader::LoadMesh_(aiMesh *m, const aiScene *scene, MeshBuilder *mesh) {
  for (unsigned int i = 0; i < m->mNumVertices; i++) {
    graphics::Vertex vertex;
    LoadVertex_(m, i, &vertex);
    mesh->AddVertex(std::move(vertex));
  }

  if (m->mMaterialIndex >= 0) {
    aiMaterial *mat = scene->mMaterials[m->mMaterialIndex];
    graphics::Material material;
    LoadMaterial_(mat, scene, &material);
    mesh->SetMaterial(std::move(material));

    // clang-format off
    LoadMaterialTextures_(mat, aiTextureType_AMBIENT, "texture_ambient", mesh);
    LoadMaterialTextures_(mat, aiTextureType_AMBIENT_OCCLUSION, "texture_ambient_occlusion", mesh);
    LoadMaterialTextures_(mat, aiTextureType_BASE_COLOR, "texture_base_color", mesh);
    LoadMaterialTextures_(mat, aiTextureType_CLEARCOAT, "texture_clearcoat", mesh);
    LoadMaterialTextures_(mat, aiTextureType_DIFFUSE, "texture_diffuse", mesh);
    LoadMaterialTextures_(mat, aiTextureType_DIFFUSE_ROUGHNESS, "texture_diffuse_roughness", mesh);
    LoadMaterialTextures_(mat, aiTextureType_DISPLACEMENT, "texture_displacement", mesh);
    LoadMaterialTextures_(mat, aiTextureType_EMISSION_COLOR, "texture_emission_color", mesh);
    LoadMaterialTextures_(mat, aiTextureType_EMISSIVE, "texture_emissive", mesh);
    LoadMaterialTextures_(mat, aiTextureType_HEIGHT, "texture_height", mesh);
    LoadMaterialTextures_(mat, aiTextureType_LIGHTMAP, "texture_lightmap", mesh);
    LoadMaterialTextures_(mat, aiTextureType_METALNESS, "texture_metalness", mesh);
    LoadMaterialTextures_(mat, aiTextureType_NORMALS, "texture_normals", mesh);
    LoadMaterialTextures_(mat, aiTextureType_NORMAL_CAMERA, "texture_normal_camera", mesh);
    LoadMaterialTextures_(mat, aiTextureType_OPACITY, "texture_opacity", mesh);
    LoadMaterialTextures_(mat, aiTextureType_REFLECTION, "texture_reflection", mesh);
    LoadMaterialTextures_(mat, aiTextureType_SHEEN, "texture_sheen", mesh);
    LoadMaterialTextures_(mat, aiTextureType_SHININESS, "texture_shininess", mesh);
    LoadMaterialTextures_(mat, aiTextureType_SPECULAR, "texture_specular", mesh);
    LoadMaterialTextures_(mat, aiTextureType_TRANSMISSION, "texture_ptrransmission", mesh);
    // clang-format on
  }

  for (unsigned int i = 0; i < m->mNumFaces; i++) {
    aiFace face = m->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      mesh->AddIndex(face.mIndices[j]);
    }
  }
}

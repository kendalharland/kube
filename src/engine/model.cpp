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

#include <kube/graphics.h>
#include <kube/logging.h>
#include <kube/model.h>
#include <kube/window.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define IDENTITY_MAT4 glm::mat4(1.f)

namespace kube {

// TODO: Move to a private common heaer.
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

// ModelLoader loads a Model from a file.
class ModelLoader {
public:
  void LoadFromFile(std::string filename, Model *model) {
    KUBE_INFO("Loading model from file " + filename);

    Assimp::Importer importer;
    auto options = aiProcess_Triangulate | aiProcess_FlipUVs; // TODO: Make these configurable.
    const aiScene *scene = importer.ReadFile(filename, options);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      KUBE_ERROR("ASSIMP::" + std::string(importer.GetErrorString()));
      throw std::runtime_error("failed to load model");
    }

    directory_ = filename.substr(0, filename.find_last_of('/'));
    ProcessNode_(scene->mRootNode, scene, model);

    // TODO: Handle shaders elsewhere!
    Shader shader;
    shader.SetVertexShader("src/shaders/TransformVertexShader.vertexshader");
    shader.SetFragmentShader("src/shaders/ColorFragmentShader.fragmentshader");
    model->shader_ = std::move(shader);
    model->center_ = glm::vec3(0.f);
    model->scale_ = glm::mat4(1.f);
    model->rotation_ = glm::mat4(1.f);
    model->Load();
  }

private:
  std::string directory_;

  void ProcessNode_(aiNode *node, const aiScene *scene, Model *model) {
    KUBE_INFO("Processing node");
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
      KUBE_INFO("Processing mesh");
      graphics::MeshBuilder mesh;
      ProcessMesh_(scene->mMeshes[node->mMeshes[i]], scene, &mesh);
      auto m = mesh.Build();
      model->meshes_.push_back(std::move(m));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
      ProcessNode_(node->mChildren[i], scene, model);
    }
  }

  void ProcessMesh_(aiMesh *m, const aiScene *scene, graphics::MeshBuilder *mesh) {
    // TODO: What are normals for, again? See book.
    // TODO: What other fields does m have that we could load?

    // vertices
    for (unsigned int i = 0; i < m->mNumVertices; i++) {
      // position
      graphics::Vertex vertex;
      glm::vec3 v3;
      v3.x = m->mVertices[i].x;
      v3.y = m->mVertices[i].y;
      v3.z = m->mVertices[i].z;
      vertex.position = std::move(v3);

      // normals
      if (m->HasNormals()) {
        glm::vec3 normal;
        normal.x = m->mNormals[i].x;
        normal.y = m->mNormals[i].y;
        normal.z = m->mNormals[i].z;
        vertex.normal = std::move(normal);
      }

      // texture coordinates
      if (m->HasTextureCoords(0)) { // does the mesh contain texture coordinates?
        glm::vec2 tex_coords;
        // a vertex can contain up to 8 different texture coordinates. We thus make the
        // assumption
        // that we won't use models where a vertex can have multiple texture coordinates so we
        // always take the first set (0).
        tex_coords.x = m->mTextureCoords[0][i].x;
        tex_coords.y = m->mTextureCoords[0][i].y;
        vertex.tex_coords = std::move(tex_coords);
      } else {
        vertex.tex_coords = glm::vec2(0.0f, 0.0f);
      }

      // Reference source code:
      // https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h
      if (m->HasTangentsAndBitangents()) {
        // tangent
        glm::vec3 tangent;
        tangent.x = m->mTangents[i].x;
        tangent.y = m->mTangents[i].y;
        tangent.z = m->mTangents[i].z;
        vertex.tangent = std::move(tangent);

        // bitangent
        glm::vec3 bitangent;
        bitangent.x = m->mBitangents[i].x;
        bitangent.y = m->mBitangents[i].y;
        bitangent.z = m->mBitangents[i].z;
        vertex.bitangent = std::move(bitangent);
      } else {
        vertex.tangent = glm::vec3(0.0f);
        vertex.bitangent = glm::vec3(0.0f);
      }

      mesh->AddVertex(std::move(vertex));
    }

    if (m->mMaterialIndex >= 0) {
      aiMaterial *material = scene->mMaterials[m->mMaterialIndex];
      // Diffuse maps.
      LoadMaterialTextures_(material, aiTextureType_DIFFUSE, "texture_diffuse", mesh);
      // Specular maps.
      LoadMaterialTextures_(material, aiTextureType_SPECULAR, "texture_specular", mesh);
    }

    // Walk through each of the faces.
    // Faces are just triangles, so each face contains the indices of the
    // vertices used to draw the face.
    for (unsigned int i = 0; i < m->mNumFaces; i++) {
      aiFace face = m->mFaces[i];
      for (unsigned int j = 0; j < face.mNumIndices; j++) {
        mesh->AddIndex(face.mIndices[j]);
      }
    }
  }

  void LoadMaterialTextures_(aiMaterial *mat, aiTextureType type, string type_name,
                             graphics::MeshBuilder *mesh) {

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
      aiString texture_path;
      mat->GetTexture(type, i, &texture_path);
      // TODO: Some textures are loaded twice. We can tell by comparing the texture_path.
      // Optimize this by only allocating new graphics::Texture objects when a unique
      // texture_path is encountered. Consider keeping a seperate vector of indices for
      // textures so we can continue using std::move.
      graphics::Texture texture;
      texture.filename = directory_ + '/' + texture_path.C_Str();
      StringReplace(&texture.filename, "\\", "/");
      texture.type = type_name;
      mesh->AddTexture(std::move(texture));
    }
  }
};

// TODO: Move implementations to model.cpp.
Model::Model(graphics::Mesh &&mesh, Shader &&shader) {
  KUBE_INFO("Loading model");
  meshes_.push_back(std::move(mesh));
  shader_ = std::move(shader);
  Load();
}

Model::~Model() {
  KUBE_INFO("Unloading model");
  Unload();
}

void Model::Load() {
  for (auto mesh : meshes_) {
    mesh.Load();
    break;
  }
  // mesh_.Load();
  shader_.Load();
}

void Model::Unload() {
  for (auto mesh : meshes_) {
    mesh.Unload();
    break;
  }
  // mesh_.Unload();
  shader_.Unload();
}

void Model::LoadFromFile(std::string filename, Model *model) {
  ModelLoader loader;
  loader.LoadFromFile(filename, model);
}

void Model::Draw(Window *window) {
  glm::mat4 translation = glm::translate(IDENTITY_MAT4, center_);
  glm::mat4 mvp = window->GetCamera().GetProjectionMatrix(translation * rotation_ * scale_);
  shader_.SetMVP(mvp);
  shader_.Use();
  for (auto mesh : meshes_) {
    mesh.Draw(&shader_);
    break;
  }
}

void Model::Rotate(float radians, glm::vec3 axis) {
  rotation_ = glm::rotate(rotation_, radians, axis);
}
}; // namespace kube

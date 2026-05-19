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

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <kube/camera.h>
#include <kube/logging.h>
#include <kube/window.h>

namespace kube {

// ============================================================================
// Vertex / vertexarray
// ============================================================================

struct Vertex {
  glm::vec3 position;
  glm::vec3 colors;
  glm::vec3 normal;
  glm::vec2 tex_coords;
  glm::vec3 tangent;
  glm::vec3 bitangent;
};

typedef struct vertexarray {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  unsigned int VAO;
  unsigned int VBO;
  unsigned int EBO;
} vertexarray;

// ============================================================================
// shader
// ============================================================================

struct ShaderSourceFiles {
  std::string vertex;
  std::string geometry;
  std::string fragment;
};

typedef struct shader {
  ShaderSourceFiles sources;
  GLuint programID;
} shader;

// ============================================================================
// texture
// ============================================================================

typedef struct texture {
  std::string filename;
  std::string type;
  unsigned int id;
} texture;

typedef std::unique_ptr<texture> texture_ptr;

// ============================================================================
// mesh
// ============================================================================

struct Material {
  glm::vec3 color_ambient;
  glm::vec3 color_emissive;
  glm::vec3 color_diffuse;
  glm::vec3 color_specular;
  float shininess;
};

typedef struct mesh {
  vertexarray vertices;
  std::vector<texture> textures;
  Material material;
} mesh;

// ============================================================================
// model
// ============================================================================

typedef struct model {
  std::string name = "";
  glm::vec3 center = glm::vec3(0);
  glm::mat4 scale = glm::mat4(1);
  glm::mat4 rotation = glm::mat4(1);
  std::vector<mesh> meshes;
} model;

// ============================================================================
// Graphics operations
// ============================================================================

void vertexArrayLoad(vertexarray &arr);
void vertexArrayUnload(const vertexarray &arr);
void vertexArrayDebugPrint(const vertexarray &arr);
void textureLoad(texture &texture);
void textureUnload(const texture &texture);
void meshLoad(mesh &mesh);
void meshUnload(const mesh &mesh);
void meshDebugPrint(const mesh &mesh);
model loadModelFromFile(std::string filename);
void modelUnload(const model &model);
void shaderLoad(shader &shader);
void shaderUnload(shader &shader);
shader shaderLoadFrom(std::filesystem::path rootPath);
void draw(const Window &window, const camera &camera, const model &model, const shader &shader);

} // namespace kube

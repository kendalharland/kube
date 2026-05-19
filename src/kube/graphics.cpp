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

#include <kube/fs.h>
#include <kube/graphics.h>
#include <kube/logging.h>
#include <kube/window.h>

#include "model_loader.h"

#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <kube/internal/stb_image.h>

const glm::mat4 IDENTITY_MAT4 = glm::mat4(1.f);
const glm::vec3 Y_AXIS = glm::vec3(0.f, 1.f, 0.f);
const glm::vec3 X_AXIS = glm::vec3(1.f, 0.f, 0.f);
const glm::vec3 Z_AXIS = glm::vec3(0.f, 0.f, 1.f);

namespace kube {
namespace kubegl {

static void useShader(const shader &shader) { glUseProgram(shader.programID); }

static void setUniformMat4(const shader &shader, const char *id, glm::mat4 value) {
  auto location = glGetUniformLocation(shader.programID, id);
  glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

static void setUniform4f(const shader &shader, const char *id, glm::vec4 value) {
  auto location = glGetUniformLocation(shader.programID, id);
  glUniform4fv(location, 1, &value[0]);
}

static void setUniform3f(const shader &shader, const char *id, glm::vec3 value) {
  auto location = glGetUniformLocation(shader.programID, id);
  glUniform3fv(location, 1, &value[0]);
}

static void setUniform2f(const shader &shader, const char *id, glm::vec2 value) {
  auto location = glGetUniformLocation(shader.programID, id);
  glUniform2fv(location, 1, &value[0]);
}

static void setUniform1f(const shader &shader, const char *id, float value) {
  auto location = glGetUniformLocation(shader.programID, id);
  glUniform1f(location, value);
}

static void bind2DTexture(const shader &shader, const texture &texture, int index) {
  auto location = glGetUniformLocation(shader.programID, texture.type.c_str());
  glActiveTexture(GL_TEXTURE0 + index);
  glUniform1i(location, index);
  glBindTexture(GL_TEXTURE_2D, texture.id);
}

static void drawVertexArray(const vertexarray &arr) {
  glBindVertexArray(arr.VAO);
  glDrawElements(GL_TRIANGLES, arr.indices.size(), GL_UNSIGNED_INT, (void *)0);
  glBindVertexArray(0);
}

static void ensureProgramLinked(GLuint program_id) {
  GLint link_status = GL_FALSE;
  int info_log_length;
  glGetProgramiv(program_id, GL_LINK_STATUS, &link_status);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::vector<char> err(info_log_length + 1);
    glGetProgramInfoLog(program_id, info_log_length, NULL, &err[0]);
    throw std::runtime_error(std::string(err.begin(), err.end()));
  }
}

static void ensureShaderCompiled(GLuint shader_id) {
  GLint link_status = GL_FALSE;
  int info_log_length;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &link_status);
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::vector<char> err(info_log_length + 1);
    glGetShaderInfoLog(shader_id, info_log_length, NULL, &err[0]);
    KUBE_ERROR << std::string(err.begin(), err.end());
  }
}

static void compileShader(GLuint shader_id, std::string filename) {
  auto source = kube::fs::readFile(filename);
  auto source_c_str = source.c_str();
  glShaderSource(shader_id, 1, &source_c_str, NULL);
  glCompileShader(shader_id);
  ensureShaderCompiled(shader_id);
}

static void linkShaderProgram(GLuint program_id) {
  glLinkProgram(program_id);
  ensureProgramLinked(program_id);
}

static GLuint compileShaderProgram(ShaderSourceFiles files) {
  // begin helper macros
#define SHADER_ID(NAME) shader_id_##NAME

#define ATTACH_SHADER_IF_SET(NAME, SHADER_TYPE)                                                    \
  GLuint SHADER_ID(NAME) = -1;                                                                     \
  if (!files.NAME.empty()) {                                                                       \
    KUBE_INFO << "Compiling " << #NAME << " shader " << files.NAME;                                \
    SHADER_ID(NAME) = glCreateShader(SHADER_TYPE);                                                 \
    compileShader(SHADER_ID(NAME), files.NAME);                                                    \
    glAttachShader(program_id, SHADER_ID(NAME));                                                   \
  }

#define DETACH_SHADER_IF_SET(NAME)                                                                 \
  if (!files.NAME.empty()) {                                                                       \
    KUBE_INFO << "Detaching shader " << #NAME;                                                     \
    glDetachShader(program_id, SHADER_ID(NAME));                                                   \
    KUBE_INFO << "Deleting shader " << #NAME;                                                      \
    glDeleteShader(SHADER_ID(NAME));                                                               \
  }
  // end helper macros.

  KUBE_INFO << "Creating shader program";
  GLuint program_id = glCreateProgram();
  ATTACH_SHADER_IF_SET(vertex, GL_VERTEX_SHADER);
  ATTACH_SHADER_IF_SET(geometry, GL_GEOMETRY_SHADER);
  ATTACH_SHADER_IF_SET(fragment, GL_FRAGMENT_SHADER);

  KUBE_INFO << "Linking shader program";
  linkShaderProgram(program_id);

  DETACH_SHADER_IF_SET(vertex);
  DETACH_SHADER_IF_SET(geometry);
  DETACH_SHADER_IF_SET(fragment);

  return program_id;
}

} // namespace kubegl

// ============================================================================
// Graphics objects
// ============================================================================

void vertexArrayLoad(vertexarray &arr) {
  glGenVertexArrays(1, &arr.VAO);
  glBindVertexArray(arr.VAO);

  glGenBuffers(1, &arr.VBO);
  glGenBuffers(1, &arr.EBO);
  glBindBuffer(GL_ARRAY_BUFFER, arr.VBO);
  glBufferData(GL_ARRAY_BUFFER, arr.vertices.size() * sizeof(Vertex), &arr.vertices[0],
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arr.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, arr.indices.size() * sizeof(unsigned int), &arr.indices[0],
               GL_STATIC_DRAW);

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
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

  // vertex texture coords
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, tex_coords));

  // vertex tangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, tangent));

  // vertex bitangent
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, bitangent));

  glBindVertexArray(0);
}

shader shaderLoadFrom(std::filesystem::path rootPath) {
  shader shader;
  shader.sources = ShaderSourceFiles{
      .vertex = rootPath / "vertex.glsl",
      .fragment = rootPath / "fragment.glsl",
  };
  KUBE_DEBUG << "Shader file: " << shader.sources.vertex;
  KUBE_DEBUG << "Shader file: " << shader.sources.fragment;
  return shader;
}

void shaderLoad(shader &shader) {
  KUBE_INFO << "Loading shader";
  shader.programID = kubegl::compileShaderProgram(shader.sources);
}

void shaderUnload(shader &shader) {
  if (shader.programID != 0) {
    KUBE_INFO << "Unloading shader " << shader.programID;
    glDeleteProgram(shader.programID);
    shader.programID = 0;
  }
}

void vertexArrayUnload(const vertexarray &arr) {
  KUBE_INFO << "Deallocating vertex array";
  glDeleteVertexArrays(1, &arr.VAO);
  glDeleteBuffers(1, &arr.VBO);
  glDeleteBuffers(1, &arr.EBO);
}

void vertexArrayDebugPrint(const vertexarray &arr) {
  // clang-format off
  KUBE_DEBUG << "mesh:" << std::endl
      << "  NumVertices: " << arr.vertices.size() << std::endl
      << "  NumIndices:" << arr.indices.size() << std::endl;
  // clang-format on
}

void textureLoad(texture &texture) {
  KUBE_INFO << "Allocating texture " << texture.filename;

  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, num_channels;
  unsigned char *data = stbi_load(texture.filename.c_str(), &width, &height, &num_channels, 0);
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
    KUBE_ERROR << "Failed to load texture from " << texture.filename;
  }

  stbi_image_free(data);
}

void textureUnload(const texture &texture) {
  KUBE_INFO << "Deallocating texture " << texture.id;
  glDeleteTextures(1, &texture.id);
}

void meshLoad(mesh &mesh) {
  vertexArrayLoad(mesh.vertices);
  // for (const texture : textures_) {
  for (auto &texture : mesh.textures) {
    textureLoad(texture);
  }
}

void meshUnload(const mesh &mesh) {
  vertexArrayUnload(mesh.vertices);
  for (auto &texture : mesh.textures) {
    textureUnload(texture);
  }
}

void meshDebugPrint(const mesh &mesh) {
  vertexArrayDebugPrint(mesh.vertices);
  // clang-format off
  KUBE_DEBUG << "  NumTextures: " << mesh.textures.size() << std::endl
      << "  Material: " << std::endl
      << "  .ColorAmbient: " << mesh.material.color_ambient.r << ", " << mesh.material.color_ambient.g << ", " << mesh.material.color_ambient.b << ")" << std::endl
      << "  .ColorEmissive: " << mesh.material.color_emissive.r << ", " << mesh.material.color_emissive.g << ", " << mesh.material.color_emissive.b << ")" << std::endl
      << "  .ColorDiffuse: " << mesh.material.color_diffuse.r << ", " << mesh.material.color_diffuse.g << ", " << mesh.material.color_diffuse.b << ")" << std::endl
      << "  .ColorSpecular: " << mesh.material.color_specular.r << ", " << mesh.material.color_specular.g << ", " << mesh.material.color_specular.b << ")" << std::endl;
  // clang-format on
}

model loadModelFromFile(std::string filename) { return ModelLoader().LoadFromFile(filename); }

void modelUnload(const model &model) {
  for (auto &mesh : model.meshes) {
    meshUnload(mesh);
  }
}

static glm::mat4 computeMVP(const camera &camera, const model &model) {
  auto mod = glm::translate(IDENTITY_MAT4, model.center) * model.rotation * model.scale;
  auto projection = glm::perspective(camera.fov, camera.aspectRatio, camera.near, camera.far);
  auto view = glm::lookAt(camera.position, camera.target, camera.up);
  return projection * view * mod;
}

void draw(const Window &window, const camera &camera, const model &model, const shader &shader) {
  kubegl::useShader(shader);
  kubegl::setUniformMat4(shader, "MVP", computeMVP(camera, model));
  kubegl::setUniform3f(shader, "u_camera_pos", camera.position);
  kubegl::setUniform3f(shader, "u_camera_tgt", camera.target);
  kubegl::setUniform2f(shader, "u_resolution", glm::vec2(1600, 1200));
  kubegl::setUniform1f(shader, "u_time", (float)glfwGetTime());

  // Draw all meshes
  for (auto &mesh : model.meshes) {
    kubegl::setUniform3f(shader, "u_light_color_diffuse", mesh.material.color_diffuse);
    kubegl::setUniform3f(shader, "u_light_color_ambient", mesh.material.color_ambient);
    kubegl::setUniform3f(shader, "u_light_color_emissive", mesh.material.color_emissive);
    kubegl::setUniform3f(shader, "u_light_color_specular", mesh.material.color_specular);

    // Bind all mesh textures
    for (int i = 0; i < mesh.textures.size(); i++) {
      kubegl::bind2DTexture(shader, mesh.textures[i], i);
    }

    // Draw all vertices
    kubegl::drawVertexArray(mesh.vertices);

    // TODO: Do this once in the loop above?
    if (!mesh.textures.empty()) {
      glActiveTexture(GL_TEXTURE0);
    }
  }
}

} // namespace kube

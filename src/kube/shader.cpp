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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <kube/logging.h>
#include <kube/shader.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace kube {
namespace graphics {

void ReadFile(std::string filename, const char **contents) {
  std::string shader_code;
  std::ifstream reader(filename, std::ios::in);
  if (!reader.is_open()) {
    throw std::runtime_error("failed to open shader file " + filename);
  }
  std::stringstream stream;
  stream << reader.rdbuf();
  shader_code = stream.str();
  reader.close();
  *contents = shader_code.c_str();
}

void EnsureProgramLinked(GLuint program_id) {
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

void EnsureShaderCompiled(GLuint shader_id) {
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

void CompileShader(GLuint shader_id, std::string filename) {
  char const *source;
  ReadFile(filename, &source);
  glShaderSource(shader_id, 1, &source, NULL);
  glCompileShader(shader_id);
  EnsureShaderCompiled(shader_id);
}

void LinkShaderProgram(GLuint program_id) {
  glLinkProgram(program_id);
  EnsureProgramLinked(program_id);
}

GLuint CompileShaderProgram(ShaderSourceFiles files) {
  // begin helper macros
#define SHADER_ID(NAME) shader_id_##NAME

#define ATTACH_SHADER_IF_SET(NAME, SHADER_TYPE)                                                    \
  GLuint SHADER_ID(NAME) = -1;                                                                     \
  if (!files.NAME.empty()) {                                                                       \
    KUBE_INFO << "Compiling " << #NAME << " shader " << files.NAME;                                \
    GLuint SHADER_ID(NAME) = glCreateShader(SHADER_TYPE);                                          \
    CompileShader(SHADER_ID(NAME), files.NAME);                                                    \
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
  LinkShaderProgram(program_id);

  DETACH_SHADER_IF_SET(vertex);
  DETACH_SHADER_IF_SET(geometry);
  DETACH_SHADER_IF_SET(fragment);

  return program_id;
}

Shader::Shader(ShaderSourceFiles shader_files) {
  shader_files_ = std::move(shader_files);
  Load();
}

Shader::~Shader() { Unload(); }

void Shader::Load() {
  KUBE_INFO << "Loading shader";
  program_id_ = CompileShaderProgram(shader_files_);
}

void Shader::Unload() {
  KUBE_INFO << "Unloading shader " << program_id_;
  glDeleteProgram(program_id_);
}

GLuint Shader::GetProgramID() { return program_id_; }

void Shader::Use() { glUseProgram(program_id_); }

void Shader::SetUniformMat4(const char *id, glm::mat4 value) {
  GLuint location = glGetUniformLocation(program_id_, id);
  glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void Shader::SetUniformVec4(const char *id, glm::vec4 value) {
  GLuint location = glGetUniformLocation(program_id_, id);
  glUniform4fv(location, 1, &value[0]);
}

shader_ptr Shader::DiffuseShader(std::string shader_source_root) {
  auto source_root = std::filesystem::path(shader_source_root);
  auto source_files = ShaderSourceFiles{
      .vertex = source_root / "DiffuseShader.vertex.glsl",
      .fragment = source_root / "DiffuseShader.fragment.glsl",
  };
  return std::make_shared<Shader>(source_files);
}

shader_ptr Shader::SimpleColorShader(std::string shader_source_root) {
  auto source_root = std::filesystem::path(shader_source_root);
  auto source_files = ShaderSourceFiles{
      .vertex = source_root / "SimpleColorShader.vertex.glsl",
      .fragment = source_root / "SimpleColorShader.fragment.glsl",
  };
  return std::make_shared<Shader>(source_files);
}

} // namespace graphics
} // namespace kube
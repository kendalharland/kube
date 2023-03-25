// Kube - A puzzle video game.
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
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <logging.h>
#include <shader.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace kube {

GLuint compile_shader(std::string filename, uint gl_shader_type) {
  KUBE_INFO("Compiling shader " + filename);

  GLuint shader_id = glCreateShader(gl_shader_type);

  // Read shader from file.
  std::string shader_code;
  std::ifstream reader(filename, std::ios::in);
  if (!reader.is_open()) {
    throw std::runtime_error("failed to open shader file" + filename);
  }
  std::stringstream stream;
  stream << reader.rdbuf();
  shader_code = stream.str();
  reader.close();

  // Compile the shader.
  char const *source = shader_code.c_str();
  glShaderSource(shader_id, 1, &source, NULL);
  glCompileShader(shader_id);

  // Check for errors.
  GLint result = GL_FALSE;
  int info_log_length;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::vector<char> err(info_log_length + 1);
    glGetShaderInfoLog(shader_id, info_log_length, NULL, &err[0]);
    KUBE_ERROR(std::string(err.begin(), err.end()));
  }

  return shader_id;
}

void Shader::SetVertexShader(std::string filename) { vertex_shader_filename_ = filename; }

void Shader::SetFragmentShader(std::string filename) { fragment_shader_filename_ = filename; }

void Shader::SetMVP(glm::mat4 mvp) {
  GLuint mvp_id_ = glGetUniformLocation(program_id_, "MVP");
  glUniformMatrix4fv(mvp_id_, 1, GL_FALSE, &mvp[0][0]);
}

void Shader::Use() { glUseProgram(program_id_); }

void Shader::Load() {
  KUBE_INFO("Loading shader program");
  vertex_shader_id_ = compile_shader(vertex_shader_filename_, GL_VERTEX_SHADER);
  fragment_shader_id_ = compile_shader(fragment_shader_filename_, GL_FRAGMENT_SHADER);

  KUBE_INFO("Linking shader program");
  GLuint program_id = glCreateProgram();
  // TODO: What if these ids are null?
  glAttachShader(program_id, vertex_shader_id_);
  glAttachShader(program_id, fragment_shader_id_);
  glLinkProgram(program_id);

  KUBE_INFO("Checking for link errors");
  GLint result = GL_FALSE;
  int info_log_length;
  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::vector<char> err(info_log_length + 1);
    glGetProgramInfoLog(program_id, info_log_length, NULL, &err[0]);
    KUBE_ERROR(std::string(err.begin(), err.end()));
  }

  KUBE_INFO("Detaching shaders");
  glDetachShader(program_id, vertex_shader_id_);
  glDetachShader(program_id, fragment_shader_id_);

  KUBE_INFO("Deleting shaders");
  glDeleteShader(vertex_shader_id_);
  glDeleteShader(fragment_shader_id_);

  program_id_ = program_id;
}

void Shader::Unload() {
  KUBE_INFO("Deleting shader program");
  glDeleteProgram(program_id_);
}

} // namespace kube
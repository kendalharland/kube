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

#include <logging.h>
#include <shader.h>
#include <stdio.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>

namespace kube {

GLuint CompileShader(std::string filename, uint gl_shader_type) {
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

  // Check Vertex Shader
  GLint result = GL_FALSE;
  int info_log_length;

  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

  if (info_log_length > 0) {
    std::vector<char> err(info_log_length + 1);
    glGetShaderInfoLog(shader_id, info_log_length, NULL, &err[0]);
    printf("%s\n", &err[0]);
  }

  return shader_id;
}

GLuint LoadShaders(const char *vertex_filename, const char *fragment_filename) {
  // Create the shaders
  GLuint vertex_shader_id = CompileShader(vertex_filename, GL_VERTEX_SHADER);
  GLuint fragment_shader_id =
      CompileShader(fragment_filename, GL_FRAGMENT_SHADER);

  // Link the program
  printf("Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, vertex_shader_id);
  glAttachShader(ProgramID, fragment_shader_id);
  glLinkProgram(ProgramID);

  GLint result = GL_FALSE;
  int info_log_length;

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::vector<char> ProgramErrorMessage(info_log_length + 1);
    glGetProgramInfoLog(ProgramID, info_log_length, NULL,
                        &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }

  glDetachShader(ProgramID, vertex_shader_id);
  glDetachShader(ProgramID, fragment_shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  return ProgramID;
}

} // namespace kube
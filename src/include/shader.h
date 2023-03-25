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

// TODO: Move this to a opengl subdirectory with opengl.h and call GL primitives
// directly.

#ifndef _SHADER_HPP
#define _SHADER_HPP

#include <glm/glm.hpp>

#include <iostream>
#include <string>

namespace kube {

typedef unsigned int GLuint;

// Represents a shader program.
class Shader {
public:
  ~Shader();
  void CompileVertexShader(std::string filename);
  void CompileFragmentShader(std::string filename);
  void LinkProgram();
  void SetMVP(glm::mat4 mvp);
  void Use();

private:
  GLuint program_id_;
  GLuint vertex_shader_id_;
  GLuint fragment_shader_id_;
};

GLuint LoadShaders(const char *vertex_filename, const char *fragment_filename);

} // namespace kube

#endif // _SHADER_HPP
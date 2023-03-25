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
  void Load();
  void Unload();
  void Use();

  void SetVertexShader(std::string filename);
  void SetFragmentShader(std::string filename);
  void SetMVP(glm::mat4 mvp);

private:
  GLuint program_id_;
  GLuint vertex_shader_id_;
  GLuint fragment_shader_id_;
  std::string vertex_shader_filename_;
  std::string fragment_shader_filename_;
};

} // namespace kube

#endif // _SHADER_HPP
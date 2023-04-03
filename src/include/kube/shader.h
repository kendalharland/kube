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
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <memory>
#include <string>

namespace kube {
namespace graphics {

struct ShaderSourceFiles {
  std::string vertex;
  std::string geometry;
  std::string fragment;
};

class Shader;
typedef std::shared_ptr<Shader> shader_ptr;

class Shader final {
public:
  static shader_ptr DiffuseShader(std::string shader_source_root);
  static shader_ptr SimpleColorShader(std::string shader_source_root);

  Shader(ShaderSourceFiles source_files);
  ~Shader();
  Shader() = delete;

  void Load();
  void Unload();

  GLuint GetProgramID();
  void Use();

  // Sets the value of a uniform mat4 of floating point values in this shader.
  void SetUniformMat4(const char *id, glm::mat4 value);
  void SetUniformVec4(const char *id, glm::vec4 value);

private:
  ShaderSourceFiles shader_files_;
  GLuint program_id_;
};

}; // namespace graphics
}; // namespace kube

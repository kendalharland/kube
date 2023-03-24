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

#ifndef _OPENGL_H
#define _OPENGL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <singleton.h>

#include <functional>
#include <glm/glm.hpp>

namespace kube {

class OpenGLContext : public Singleton<OpenGLContext> {
 private:
  SINGLETON(OpenGLContext) {}

  GLuint vertex_array_;

 public:  // TODO: Remove
  GLFWwindow* window_;

 public:
  void CreateWindow(int width, int height, const char* title);
  void CloseWindow();
  bool IsKeyPressed(uint key);
  bool WindowShouldClose();
  void Clear();
  void Update();
};

};  // namespace kube

#endif  // _OPENGL_H
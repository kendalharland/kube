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

#include <functional>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <kube/camera.h>
#include <kube/singleton.h>

namespace kube {

// Window opens a window on the host machine.
class Window final : public Singleton<Window> {
private:
  SINGLETON(Window) : is_open_(false) {}

  static void glfw_scroll(GLFWwindow *window, double xoffset, double yoffset) {
    Window *instance = Window::GetInstance();
    instance->camera_->Zoom(yoffset > 0);
  }

public:
  void Open(int width, int height, const char *title);
  void Close();
  void Clear();
  void Update();
  void SetCamera(Camera *camera);

  bool IsKeyPressed(uint key);
  bool ShouldClose();

private:
  bool is_open_;
  GLFWwindow *window_;
  Camera *camera_;
  GLuint VAO_;
};

}; // namespace kube

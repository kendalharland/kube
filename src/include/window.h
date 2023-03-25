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

#ifndef _WINDOW_HPP
#define _WINDOW_HPP

#include <functional>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <camera.h>
#include <graphics.h>
#include <logging.h>
#include <model.h>
#include <singleton.h>

namespace kube {

class Window : public Singleton<Window> {
private:
  SINGLETON(Window) : is_open_(false) {}

  static void glfw_scroll(GLFWwindow *window, double xoffset, double yoffset) {
    Window *instance = Window::GetInstance();
    instance->camera_.Zoom(yoffset > 0);
  }

public:
  void Open(int width, int height, const char *title);
  void Close();
  void Clear();
  void Update();

  void SetCamera(Camera &&camera);
  Camera &GetCamera();

  bool IsKeyPressed(uint key);
  bool ShouldClose();

private:
  bool is_open_;
  GLFWwindow *window_;
  Camera camera_;
  GLuint vertex_array_id_; // TODO: Delete?

  // TODO: delete singleton constructors and assignment operators.
};

}; // namespace kube

#endif // _WINDOW_HPP
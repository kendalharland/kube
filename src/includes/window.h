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

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>

namespace kube {

class Window {
 public:
  static Window* GetInstance() {
    if (instance_ == nullptr) {
      instance_ = new Window();
    }

    return instance_;
  };

  void Init(int width, int height, const char* title);
  void Clear();
  void Update();
  void SetScrollCallback(std::function<void(double, double)> callback);
  bool ShouldClose();
  void Close();
  GLFWwindow* inner() { return _window; }

 private:
  static Window* instance_;

  GLFWwindow* _window;
  GLuint _vertex_array_id;
  std::function<void(double, double)> scroll_callback_;

  Window(){};
  static void GLFWScrollCallback(GLFWwindow* window, double xoffset,
                                 double yoffset) {
    GetInstance()->scroll_callback_(xoffset, yoffset);
  }

  // TODO: delete singleton constructors and assignment operators.
};

};  // namespace kube

#endif  // _WINDOW_HPP
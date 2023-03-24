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

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <window.h>

#include <glm/glm.hpp>

namespace kube {

void Window::Open(int width, int height, const char* title) {
  if (is_open_) {
    throw "Window::Open called twice";
  }
  is_open_ = true;
  opengl_->CreateWindow(width, height, title);
};

void Window::SetScrollCallback(std::function<void(double, double)> callback) {
  scroll_callback_ = callback;
}

void Window::Clear() {
  opengl_->Clear();
};

void Window::Update() {
  opengl_->Update();
};

bool Window::ShouldClose() {
  return opengl_->IsKeyPressed(GLFW_KEY_ESCAPE) || opengl_->WindowShouldClose();
};

void Window::Close() {
  opengl_->CloseWindow();
}

}  // namespace kube
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

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <kube/camera.h>

namespace kube {

struct Window {
  bool is_open = false;
  GLFWwindow *glfw_window = nullptr;
  camera *active_camera = nullptr;
  GLuint VAO = 0;
};

void windowOpen(std::shared_ptr<Window> window, int width, int height, const char *title);
void windowClose(std::shared_ptr<Window> window);
void windowClear(std::shared_ptr<Window> window);
void windowUpdate(std::shared_ptr<Window> window);
bool windowShouldClose(std::shared_ptr<Window> window);
bool windowIsKeyPressed(std::shared_ptr<Window> window, uint key);
void windowSetCamera(std::shared_ptr<Window> window, camera *camera);

} // namespace kube

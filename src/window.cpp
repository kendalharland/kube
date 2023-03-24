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

void Window::Init(int width, int height, const char* title) {
  if (!glfwInit()) {
    throw "Failed to initialize GLFW\n";
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // GL_TRUE makes MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  _window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (_window == NULL) {
    glfwTerminate();
    throw(
        "Failed to open GLFW window. If you have an Intel GPU, they are "
        "not 3.3 compatible. Try version 2.1");
  }

  glfwMakeContextCurrent(_window);

  glfwSetScrollCallback(_window, Window::GLFWScrollCallback);

  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    throw "Failed to initialize GLEW\n";
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_FALSE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  glGenVertexArrays(1, &_vertex_array_id);
  glBindVertexArray(_vertex_array_id);
};

void Window::SetScrollCallback(std::function<void(double, double)> callback) {
  scroll_callback_ = callback;
}

void Window::Clear() {
  // Save the initial ModelView matrix before modifying ModelView matrix.
  glPushMatrix();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void Window::Update() {
  // Restore initial ModelView matrix.
  glPopMatrix();
  glfwSwapBuffers(_window);
  glfwPollEvents();
};

bool Window::ShouldClose() {
  return glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
         glfwWindowShouldClose(_window) == 1;
};

void Window::Close() {
  // Clean VBO
  glDeleteVertexArrays(1, &_vertex_array_id);
  glfwTerminate();
}

}  // namespace kube
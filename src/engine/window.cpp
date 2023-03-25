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

#include <memory>

#include <kube/camera.h>
#include <kube/model.h>
#include <kube/window.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace kube {

void Window::SetCamera(Camera &&camera) {
  assert(is_open_);
  camera_ = std::move(camera);
  glfwSetScrollCallback(window_, Window::glfw_scroll);
}

Camera &Window::GetCamera() { return camera_; }

void Window::Open(int width, int height, const char *title) {
  assert(!is_open_);
  is_open_ = true;

  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // GL_TRUE makes MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window_ = glfwCreateWindow(width, height, title, NULL, NULL);
  if (window_ == NULL) {
    glfwTerminate();
    throw std::runtime_error("Failed to open GLFW window. If you have an Intel GPU, they are not "
                             "3.3 compatible. Try version 2.1");
  }

  glfwMakeContextCurrent(window_);

  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    throw std::runtime_error("Failed to initialize GLEW");
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_FALSE);

  // TODO: Make this configurable.
  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  glGenVertexArrays(1, &vertex_array_id_);
  glBindVertexArray(vertex_array_id_);
};

void Window::Clear() {
  // Save the initial ModelView matrix before modifying ModelView matrix.
  glPushMatrix();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void Window::Update() {
  // Restore initial ModelView matrix.
  glPopMatrix();
  glfwSwapBuffers(window_);
  glfwPollEvents();
};

bool Window::IsKeyPressed(uint key) { return glfwGetKey(window_, key) == GLFW_PRESS; }

bool Window::ShouldClose() {
  return IsKeyPressed(GLFW_KEY_ESCAPE) || glfwWindowShouldClose(window_);
}

void Window::Close() {
  // Clean VBO
  glDeleteVertexArrays(1, &vertex_array_id_);
  glfwTerminate();
}

}; // namespace kube
   // namespace kube
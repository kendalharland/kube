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

#include <cassert>
#include <memory>
#include <stdexcept>

#include <kube/camera.h>
#include <kube/window.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace kube {

static void glfw_scroll(GLFWwindow *glfw_window, double xoffset, double yoffset) {
  auto *window = static_cast<Window *>(glfwGetWindowUserPointer(glfw_window));
  if (window->active_camera) {
    cameraZoom(*window->active_camera, yoffset > 0);
  }
}

void windowOpen(std::shared_ptr<Window> window, int width, int height, const char *title) {
  assert(!window->is_open);
  window->is_open = true;

  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window->glfw_window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (window->glfw_window == NULL) {
    glfwTerminate();
    throw std::runtime_error("Failed to open GLFW window. If you have an Intel GPU, they are not "
                             "3.3 compatible. Try version 2.1");
  }

  glfwMakeContextCurrent(window->glfw_window);
  glfwSetWindowUserPointer(window->glfw_window, window.get());
  glfwSetScrollCallback(window->glfw_window, glfw_scroll);

  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    throw std::runtime_error("Failed to initialize GLEW");
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glfwSetInputMode(window->glfw_window, GLFW_STICKY_KEYS, GL_FALSE);

  // TODO: Make this configurable.
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  glGenVertexArrays(1, &window->VAO);
  glBindVertexArray(window->VAO);
}

void windowClose(std::shared_ptr<Window> window) {
  glDeleteVertexArrays(1, &window->VAO);
  glfwTerminate();
}

void windowClear(std::shared_ptr<Window> window) {
  glPushMatrix();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void windowUpdate(std::shared_ptr<Window> window) {
  glPopMatrix();
  glfwSwapBuffers(window->glfw_window);
  glfwPollEvents();
}

bool windowIsKeyPressed(std::shared_ptr<Window> window, uint key) {
  return glfwGetKey(window->glfw_window, key) == GLFW_PRESS;
}

bool windowShouldClose(std::shared_ptr<Window> window) {
  return windowIsKeyPressed(window, GLFW_KEY_ESCAPE) ||
         glfwWindowShouldClose(window->glfw_window);
}

void windowSetCamera(std::shared_ptr<Window> window, camera *camera) {
  assert(window->is_open);
  window->active_camera = camera;
}

} // namespace kube

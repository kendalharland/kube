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
#include <mesh.h>
#include <opengl.h>

#include <glm/glm.hpp>

namespace kube {

void OpenGLContext::CreateWindow(int width, int height, const char *title) {
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
  window_ = glfwCreateWindow(width, height, title, NULL, NULL);
  if (window_ == NULL) {
    glfwTerminate();
    throw("Failed to open GLFW window. If you have an Intel GPU, they are "
          "not 3.3 compatible. Try version 2.1");
  }

  glfwMakeContextCurrent(window_);

  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    throw "Failed to initialize GLEW\n";
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_FALSE);

  // TODO: Make this configurable.
  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
  glGenVertexArrays(1, &vertex_array_);
  glBindVertexArray(vertex_array_);
}

void OpenGLContext::CloseWindow() {
  // Clean VBO
  glDeleteVertexArrays(1, &vertex_array_);
  glfwTerminate();
}

void OpenGLContext::Clear() {
  // Save the initial ModelView matrix before modifying ModelView matrix.
  glPushMatrix();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void OpenGLContext::Update() {
  // Restore initial ModelView matrix.
  glPopMatrix();
  glfwSwapBuffers(window_);
  glfwPollEvents();
};

bool OpenGLContext::IsKeyPressed(uint key) { return glfwGetKey(window_, key) == GLFW_PRESS; }

bool OpenGLContext::WindowShouldClose() { return glfwWindowShouldClose(window_); }

// clang-format off
void OpenGLContext::SetupMesh(Mesh &mesh) {
  unsigned int VAO; // vertex arrays
  unsigned int VBO; // vertex buffer
  unsigned int EBO; // element array buffer

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tex_coords));

  glBindVertexArray(0);

  mesh.SetVAO(VAO);
  mesh.SetVBO(VBO);
  mesh.SetEBO(EBO);
}
// clang-format on

}; // namespace kube
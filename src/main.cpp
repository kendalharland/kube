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
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>  // For debugging.

#include "glm/gtx/string_cast.hpp"
using namespace glm;

#include "camera.hpp"
#include "constants.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "shapes.hpp"
#include "vertex_shader.hpp"

GLFWwindow* window;

auto camera = kube::Camera(CAMERA_POS, CAMERA_TARGET);

void DrawModel(kube::Camera camera, kube::Model* model,
               kube::ModelShader& shader) {
  glm::mat4 translation = glm::translate(IDENTITY_MAT4, model->Center());
  glm::mat4 rotation = model->Rotation();
  glm::mat4 scale = model->Scale();
  glm::mat4 mvp = camera.MVP(translation * rotation * scale);
  shader.Draw(mvp, model->Vertices(), model->NumVertices(), model->Colors(),
              model->NumColors(), model->Indices(), model->NumIndices());
}

void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.Zoom(yoffset > 0);
}

int main(void) {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
                 GL_TRUE);  // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_WIDTH, TITLE, NULL, NULL);
  if (window == NULL) {
    fprintf(stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are "
            "not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return -1;
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Camera configuration
  glfwSetScrollCallback(window, mouseScrollCallback);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // TODO(kjharland): This should probably be moved to the Shader type?.
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Must come after the VertexArray above is created.
  auto shader = kube::ModelShader();
  auto playerCube = kube::Cube(glm::vec3(0, 0, 0), 2.f);
  auto tile = kube::Cube(glm::vec3(0, -2, 0), 1.f);

  double currentTime = glfwGetTime();
  double lastTime = currentTime;
  double deltaTime = 0;

  do {
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Save the initial ModelView matrix before modifying ModelView matrix.
    glPushMatrix();

    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause
    // flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
      playerCube->RotateRight(deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
      playerCube->RotateLeft(deltaTime);
    }

    DrawModel(camera, playerCube, shader);
    DrawModel(camera, tile, shader);

    // Restore initial MovelView matrix.
    glPopMatrix();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }  // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0);

  // Clean VBO
  glDeleteVertexArrays(1, &VertexArrayID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

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
using namespace glm;

#include "camera.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "shapes.hpp"
#include "vertex_shader.hpp"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define TITLE "Kube"
#define TARGET_FPS 60

#define TILE_SIZE 1.f
#define CUBE_SIZE 1.f

GLFWwindow* window;

void DrawModel(kube::Camera camera, kube::Model* model,
               kube::ModelShader& shader) {
  glm::mat4 mvp = camera.Project(model->Center());
  shader.Draw(mvp, model->Vertices(), model->NumVertices(), model->Colors(),
              model->NumColors(), model->Indices(), model->NumIndices());
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

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  // // TODO(kjharland): This should probably be moved to the Shader type?.
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  // Must come after the VertexArray above is created.
  auto shader = kube::ModelShader();
  auto cube = kube::Cube(glm::vec3(0.f));

  auto camera = kube::Camera(
      glm::vec3(4, 3, -3),  // Camera is at (4,3,-3), in World Space
      glm::vec3(0, 0, 0)    // and looks at the origin
  );

  do {
    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause
    // flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Save the initial ModelView matrix before modifying ModelView matrix.
    glPushMatrix();

    DrawModel(camera, cube, shader);

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

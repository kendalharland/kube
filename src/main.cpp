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

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "shader.hpp"

#include "cube.hpp"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define TITLE "Kube"
#define TARGET_FPS 60

#define TILE_SIZE 1.f
#define CUBE_SIZE 1.f

GLFWwindow *window;

// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// clang-format off
static const GLfloat vertices[] = {
   1, 1, 1,   -1, 1, 1,   -1,-1, 1,    1,-1, 1, // v0,v1,v2,v3 (front)
   1, 1, 1,    1,-1, 1,    1,-1,-1,    1, 1,-1, // v0,v3,v4,v5 (right)
   1, 1, 1,    1, 1,-1,   -1, 1,-1,   -1, 1, 1, // v0,v5,v6,v1 (top)
  -1, 1, 1,   -1, 1,-1,   -1,-1,-1,   -1,-1, 1, // v1,v6,v7,v2 (left)
  -1,-1,-1,    1,-1,-1,    1,-1, 1,   -1,-1, 1, // v7,v4,v3,v2 (bottom)
   1,-1,-1,   -1,-1,-1,   -1, 1,-1,    1, 1,-1  // v4,v7,v6,v5 (back)
};

GLfloat normals[] = {
   0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1, // v0,v1,v2,v3 (front)
   1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0, // v0,v3,v4,v5 (right)
   0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0, // v0,v5,v6,v1 (top)
  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0, // v1,v6,v7,v2 (left)
   0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0, // v7,v4,v3,v2 (bottom)
   0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1  // v4,v7,v6,v5 (back)
};

GLfloat colors[] = {
  1, 1, 1,   1, 1, 0,   1, 0, 0,   1, 0, 1, // v0,v1,v2,v3 (front)
  1, 1, 1,   1, 0, 1,   0, 0, 1,   0, 1, 1, // v0,v3,v4,v5 (right)
  1, 1, 1,   0, 1, 1,   0, 1, 0,   1, 1, 0, // v0,v5,v6,v1 (top)
  1, 1, 0,   0, 1, 0,   0, 0, 0,   1, 0, 0, // v1,v6,v7,v2 (left)
  0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0, // v7,v4,v3,v2 (bottom)
  0, 0, 1,   0, 0, 0,   0, 1, 0,   0, 1, 1  // v4,v7,v6,v5 (back)
}; 

GLubyte indices[]  = {
   0, 1, 2,   2, 3, 0, // front                        
   4, 5, 6,   6, 7, 4, // right                        
   8, 9,10,  10,11, 8, // top                          
  12,13,14,  14,15,12, // left                         
  16,17,18,  18,19,16, // bottom                       
  20,21,22,  22,23,20  // back
};
// clang-format on

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
                 GL_TRUE); // To make MacOS happy; should not be needed
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

  GLuint programID = LoadShaders("src/TransformVertexShader.vertexshader",
                                 "src/ColorFragmentShader.fragmentshader");

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GLuint vertexBuffer;
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint colorBuffer;
  glGenBuffers(1, &colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  // Get a handle for our "MVP" uniform
  GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit
  // <-> 100 units
  glm::mat4 Projection =
      glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

  // Camera matrix
  glm::mat4 View = glm::lookAt(
      glm::vec3(4, 3, -3), // Camera is at (4,3,-3), in World Space
      glm::vec3(0, 0, 0),  // and looks at the origin
      glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
  );

  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model = glm::mat4(1.0f);

  // Our ModelViewProjection : multiplication of our 3 matrices
  // Remember, matrix multiplication is the other way around
  glm::mat4 MVP = Projection * View * Model;

  do {
    // Clear the screen. It's not mentioned before Tutorial 02, but it can cause
    // flickering, so it's there nonetheless.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Save the initial ModelView matrix before modifying ModelView matrix.
    glPushMatrix();

    // transform camera
    float cameraAngleX = 0;
    float cameraAngleY = 0;
    glTranslatef(0, 0, -10.0f);
    glRotatef(cameraAngleX, 1, 0, 0);
    glRotatef(cameraAngleY, 0, 1, 0);

    glUseProgram(programID);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Create our shape
    // 1st attribute buffer : vertices.
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // enable and specify pointers to vertex arrays
    // glEnableClientState(GL_NORMAL_ARRAY);
    // glNormalPointer(GL_FLOAT, 0, normals);

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glPushMatrix();
    glTranslatef(-2, -2, 0); // move to bottom-left corner
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
    glPopMatrix();

    glDisableClientState(GL_VERTEX_ARRAY); // disable vertex arrays
    glDisableClientState(GL_COLOR_ARRAY);
    // glDisableClientState(GL_NORMAL_ARRAY);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    // Restore initial MovelView matrix.
    glPopMatrix();

    glfwSwapBuffers(window);
    glfwPollEvents();
  } // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0);

  // Clean VBO
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteBuffers(1, &colorBuffer);
  glDeleteVertexArrays(1, &VertexArrayID);
  glDeleteProgram(programID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

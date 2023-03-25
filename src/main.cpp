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
#include <iostream> // For debugging.

using namespace glm;

#include <animation.hpp>
#include <camera.cpp>
#include <constants.hpp>
#include <logging.h>
#include <mesh.h>
#include <model.hpp>
#include <player.h>
#include <shapes.cpp>
#include <vertex_shader.cpp>
#include <window.h>

void DrawModel(kube::Camera camera, kube::DeprecatedModel *model, kube::ModelShader &shader) {}

int main(void) {
  auto window = kube::Window::GetInstance();
  auto camera = kube::Camera(CAMERA_POS, CAMERA_TARGET);

  window->Open(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  window->SetScrollCallback(
      [&camera = camera](double xoffset, double yoffset) { camera.Zoom(yoffset > 0); });

  // Must come after the VertexArray above is created.
  auto shader = kube::Shader();
  shader.CompileVertexShader("src/shaders/TransformVertexShader.vertexshader");
  shader.CompileFragmentShader("src/shaders/ColorFragmentShader.fragmentshader");
  shader.LinkProgram();

  double currentTime = glfwGetTime();
  double lastTime = currentTime;
  double deltaTime = 0;

  auto model = kube::Cube(glm::vec3(0, 0, 0), 2.f);
  auto state = kube::PlayerIdleState();
  kube::Player player(&model, &state);

  auto mesh = model.CreateMesh();
  window->opengl()->SetupMesh(*mesh);

  do {
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    player.HandleInput(window->inner());
    player.Update(deltaTime);
    window->Clear();

    glm::mat4 translation = glm::translate(IDENTITY_MAT4, model.Center());
    glm::mat4 rotation = model.Rotation();
    glm::mat4 scale = model.Scale();
    glm::mat4 mvp = camera.MVP(translation * rotation * scale);
    window->opengl()->DrawMesh(*mesh, shader, mvp);

    window->Update();
  } // Check if the ESC key was pressed or the window was closed
  while (!window->ShouldClose());

  window->opengl()->TeardownMesh(*mesh);
  window->Close();

  return 0;
}

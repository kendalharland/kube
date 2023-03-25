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

#include <iostream> // For debugging.
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <animation.hpp>
#include <camera.h>
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
  kube::Camera camera(CAMERA_POS, CAMERA_TARGET);

  window->Open(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  window->SetCamera(std::move(camera));

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
  kube::graphics::LoadMesh(*mesh);

  do {
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    player.HandleInput(window);
    player.Update(deltaTime);
    window->Clear();
    window->DrawMesh(mesh);
    glm::mat4 translation = glm::translate(IDENTITY_MAT4, model.Center());
    glm::mat4 rotation = model.Rotation();
    glm::mat4 scale = model.Scale();
    glm::mat4 mvp = camera.MVP(translation * rotation * scale);
    kube::graphics::DrawMesh(*mesh, shader, mvp);

    window->Update();
  } // Check if the ESC key was pressed or the window was closed
  while (!window->ShouldClose());

  kube::graphics::UnloadMesh(*mesh);
  window->Close();

  return 0;
}

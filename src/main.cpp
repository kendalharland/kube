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

using namespace glm;

#include <player.h>
#include <vertex.h>
#include <window.h>

#include <animation.hpp>
#include <assimp/Importer.hpp>
#include <camera.cpp>
#include <constants.hpp>
#include <model.hpp>
#include <shapes.cpp>
#include <vertex_shader.cpp>

void DrawModel(kube::Camera camera, kube::DeprecatedModel* model,
               kube::ModelShader& shader) {
  glm::mat4 translation = glm::translate(IDENTITY_MAT4, model->Center());
  glm::mat4 rotation = model->Rotation();
  glm::mat4 scale = model->Scale();
  glm::mat4 mvp = camera.MVP(translation * rotation * scale);
  shader.Draw(mvp, model->Vertices(), model->NumVertices(), model->Colors(),
              model->NumColors(), model->Indices(), model->NumIndices());
}

int main(void) {
  auto window = kube::Window::GetInstance();
  auto camera = kube::Camera(CAMERA_POS, CAMERA_TARGET);

  auto vert = kube::Vertex{
      .position = glm::vec3(1, 2, 3),
      .color = glm::vec3(1, 1, 0),
  };

  std::cout << sizeof(vert.position) << std::endl;

  window->Open(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
  window->SetScrollCallback([&camera = camera](double xoffset, double yoffset) {
    camera.Zoom(yoffset > 0);
  });

  // Must come after the VertexArray above is created.
  auto shader = kube::ModelShader();

  double currentTime = glfwGetTime();
  double lastTime = currentTime;
  double deltaTime = 0;

  auto model = kube::Cube(glm::vec3(0, 0, 0), 2.f);
  auto state = kube::PlayerIdleState();
  kube::Player player(&model, &state);

  do {
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    player.HandleInput(window->inner());
    player.Update(deltaTime);
    window->Clear();
    DrawModel(camera, &model, shader);
    window->Update();
  }  // Check if the ESC key was pressed or the window was closed
  while (!window->ShouldClose());

  window->Close();

  return 0;
}

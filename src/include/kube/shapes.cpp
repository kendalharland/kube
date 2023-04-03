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

#pragma once

#include <memory>

#include <kube/mesh.h>
#include <kube/model.h>
#include <kube/shader.h>

#include <glm/glm.hpp>

namespace kube {

// clang-format off

//
//        y
//        |
//        +--x
//       /
//      z
//
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
static const GLfloat cube_vertices_[] = {
   1, 1, 1, // v0
  -1, 1, 1, // v1
  -1,-1, 1, // v2
   1,-1, 1, // v3
   1,-1,-1, // v4
   1, 1,-1, // v5
  -1, 1,-1, // v6
  -1,-1,-1  // v7
};

// Creates a cube whose vertices transition between the full visbile
// spectrum of visibile colors.
static const GLfloat cube_colors_[] = {
  1, 1, 1,   1, 1, 0,   1, 0, 0,   1, 0, 1, // v0,v1,v2,v3 (front)
  1, 1, 1,   1, 0, 1,   0, 0, 1,   0, 1, 1, // v0,v3,v4,v5 (right)
};

static unsigned int cube_indices_[]  = {
   0, 1, 2, 2, 3, 0, // front
   5, 0, 3, 3, 4, 5, // right
   5, 6, 7, 7, 4, 5, // back
   6, 1, 2, 2, 7, 6, // left
   5, 6, 1, 1, 0, 5, // top
   4, 7, 2, 2, 3, 4, // bottom
};
// clang-format on

// Generic, drawable cube geometry.
std::shared_ptr<Model> CreateCubeModel() {
  std::vector<graphics::Vertex> vertices;
  std::vector<unsigned int> indices;

  for (size_t i = 0; i < sizeof(cube_vertices_) / sizeof(GLfloat); i += 3) {
    graphics::Vertex v;
    v.position.x = cube_vertices_[i];
    v.position.y = cube_vertices_[i + 1];
    v.position.z = cube_vertices_[i + 2];

    v.colors.x = cube_colors_[i];
    v.colors.y = cube_colors_[i + 1];
    v.colors.z = cube_colors_[i + 2];

    vertices.push_back(v);
  }

  for (size_t i = 0; i < sizeof(cube_indices_) / sizeof(unsigned int); i++) {
    indices.push_back(cube_indices_[i]);
  }

  auto vertex_array = std::make_unique<graphics::VertexArray>(vertices, indices);
  auto mesh = std::make_unique<graphics::Mesh>(std::move(vertex_array));
  return std::make_shared<Model>(std::move(mesh));
}

}; // namespace kube

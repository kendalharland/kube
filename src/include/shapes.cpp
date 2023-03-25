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

#ifndef _SHAPES_H
#define _SHAPES_H

#include <constants.hpp>
#include <graphics.h>
#include <model.h>
#include <shader.h>

#include <glm/glm.hpp>

namespace kube {

// clang-format off

//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
static const GLfloat cube_vertices_[] = {
  1, 1, 1,   -1, 1, 1,   -1,-1, 1,    1,-1, 1, // v0,v1,v2,v3 (front)
  1, 1, 1,    1,-1, 1,    1,-1,-1,    1, 1,-1, // v0,v3,v4,v5 (right)
  1, 1, 1,    1, 1,-1,   -1, 1,-1,   -1, 1, 1, // v0,v5,v6,v1 (top)
 -1, 1, 1,   -1, 1,-1,   -1,-1,-1,   -1,-1, 1, // v1,v6,v7,v2 (left)
 -1,-1,-1,    1,-1,-1,    1,-1, 1,   -1,-1, 1, // v7,v4,v3,v2 (bottom)
  1,-1,-1,   -1,-1,-1,   -1, 1,-1,    1, 1,-1  // v4,v7,v6,v5 (back)
};

// Creates a cube whose vertices transition between the full visbile
// spectrum of visibile colors.
static const GLfloat cube_colors_[] = {
  1, 1, 1,   1, 1, 0,   1, 0, 0,   1, 0, 1, // v0,v1,v2,v3 (front)
  1, 1, 1,   1, 0, 1,   0, 0, 1,   0, 1, 1, // v0,v3,v4,v5 (right)
  1, 1, 1,   0, 1, 1,   0, 1, 0,   1, 1, 0, // v0,v5,v6,v1 (top)
  1, 1, 0,   0, 1, 0,   0, 0, 0,   1, 0, 0, // v1,v6,v7,v2 (left)
  0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0, // v7,v4,v3,v2 (bottom)
  0, 0, 1,   0, 0, 0,   0, 1, 0,   0, 1, 1  // v4,v7,v6,v5 (back)
}; 

static unsigned int cube_indices_[]  = {
    0, 1, 2,   2, 3, 0, // front                        
    4, 5, 6,   6, 7, 4, // right                        
    8, 9,10,  10,11, 8, // top                          
   12,13,14,  14,15,12, // left                         
   16,17,18,  18,19,16, // bottom                       
   20,21,22,  22,23,20  // back
};
// clang-format on

// Generic, drawable cube geometry.
Model Cube(glm::vec3 center, float length = 1.f) {
  std::vector<graphics::Vertex> vertices;
  std::vector<unsigned int> indices;

  for (int i = 0; i < sizeof(cube_vertices_); i += 3) {
    graphics::Vertex v;
    v.position.x = cube_vertices_[i];
    v.position.y = cube_vertices_[i + 1];
    v.position.z = cube_vertices_[i + 2];

    v.colors.x = cube_colors_[i];
    v.colors.y = cube_colors_[i + 1];
    v.colors.z = cube_colors_[i + 2];
    vertices.push_back(v);
  }

  for (int i = 0; i < sizeof(cube_indices_); i++) {
    indices.push_back(cube_indices_[i]);
  }

  graphics::Mesh mesh(vertices, indices);

  Shader shader;
  shader.SetVertexShader("src/shaders/TransformVertexShader.vertexshader");
  shader.SetFragmentShader("src/shaders/ColorFragmentShader.fragmentshader");

  Model model(std::move(mesh), std::move(shader));
  return model;
}

}; // namespace kube

#endif // _SHAPES_H
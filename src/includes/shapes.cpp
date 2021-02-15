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

#ifndef _SHAPES_HPP
#define _SHAPES_HPP

#include <constants.hpp>
#include <glm/glm.hpp>
#include <model.hpp>

namespace kube {

// clang-format off

//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3
static const GLfloat vertices[] = {
  1, 1, 1,   -1, 1, 1,   -1,-1, 1,    1,-1, 1, // v0,v1,v2,v3 (front)
  1, 1, 1,    1,-1, 1,    1,-1,-1,    1, 1,-1, // v0,v3,v4,v5 (right)
  1, 1, 1,    1, 1,-1,   -1, 1,-1,   -1, 1, 1, // v0,v5,v6,v1 (top)
 -1, 1, 1,   -1, 1,-1,   -1,-1,-1,   -1,-1, 1, // v1,v6,v7,v2 (left)
 -1,-1,-1,    1,-1,-1,    1,-1, 1,   -1,-1, 1, // v7,v4,v3,v2 (bottom)
  1,-1,-1,   -1,-1,-1,   -1, 1,-1,    1, 1,-1  // v4,v7,v6,v5 (back)
};

static const GLfloat colors[] = {
  1, 1, 1,   1, 1, 0,   1, 0, 0,   1, 0, 1, // v0,v1,v2,v3 (front)
  1, 1, 1,   1, 0, 1,   0, 0, 1,   0, 1, 1, // v0,v3,v4,v5 (right)
  1, 1, 1,   0, 1, 1,   0, 1, 0,   1, 1, 0, // v0,v5,v6,v1 (top)
  1, 1, 0,   0, 1, 0,   0, 0, 0,   1, 0, 0, // v1,v6,v7,v2 (left)
  0, 0, 0,   0, 0, 1,   1, 0, 1,   1, 0, 0, // v7,v4,v3,v2 (bottom)
  0, 0, 1,   0, 0, 0,   0, 1, 0,   0, 1, 1  // v4,v7,v6,v5 (back)
}; 

static const GLubyte indices[]  = {
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
  return Model({
      .center = center,
      .colors = colors,
      .indices = &indices[0],
      .numColors = sizeof(colors),
      .numIndices = sizeof(indices),
      .numVertices = sizeof(vertices),
      .scale = glm::scale(IDENTITY_MAT4, glm::vec3(length)),
      .vertices = vertices,
  });
}

};  // namespace kube

#endif  // _SHAPES_HPP
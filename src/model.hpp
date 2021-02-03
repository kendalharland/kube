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

#ifndef _MODEL_HPP
#define _MODEL_HPP

#define ROTATIONS_PER_SEC 90.f

static const glm::vec3 Y_AXIS = glm::vec3(0.f, 1.f, 0.f);

#include <glm/glm.hpp>

#include "vertex_shader.hpp"

namespace kube {

class Model {
 public:
  Model() = delete;

  // Constructs a Model with the given center position.
  //
  // shader is used when drawing the model.
  Model(glm::vec3 center, const GLfloat *vertices, int numVertices,
        const GLfloat *colors, int numColors, const GLubyte *indices,
        int numIndices)
      : _center(center),
        _colors(colors),
        _indices(indices),
        _numColors(numColors),
        _numIndices(numIndices),
        _numVertices(numVertices),
        _rotation(glm::mat4(1.f)),
        _vertices(vertices) {}

  const glm::vec3 Center() { return _center; }

  const glm::mat4 Rotation() { return _rotation; }

  const GLfloat *Vertices() { return _vertices; }
  int NumVertices() { return _numVertices; }

  const GLfloat *Colors() { return _colors; }
  int NumColors() { return _numColors; }

  const GLubyte *Indices() { return _indices; }
  int NumIndices() { return _numIndices; }

  void RotateRight(float t) {
    auto angle = glm::radians(ROTATIONS_PER_SEC) * t;
    _rotation = glm::rotate(_rotation, angle, -Y_AXIS);
  }

  void RotateLeft(float t) {
    auto angle = glm::radians(ROTATIONS_PER_SEC) * t;
    _rotation = glm::rotate(_rotation, angle, Y_AXIS);
  }

 private:
  const GLfloat *_vertices;
  int _numVertices;

  const GLfloat *_colors;
  int _numColors;

  const GLubyte *_indices;
  int _numIndices;

  glm::vec3 _center;
  glm::mat4 _rotation;
};

}  // namespace kube

#endif  // _MODEL_HPP
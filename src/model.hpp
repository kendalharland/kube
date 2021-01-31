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
        _vertices(vertices),
        _numVertices(numVertices),
        _colors(colors),
        _numColors(numColors),
        _indices(indices),
        _numIndices(numIndices) {}

  glm::vec3 Center() { return _center; }

  const GLfloat *Vertices() { return _vertices; }
  int NumVertices() { return _numVertices; }

  const GLfloat *Colors() { return _colors; }
  int NumColors() { return _numColors; }

  const GLubyte *Indices() { return _indices; }
  int NumIndices() { return _numIndices; }

 private:
  const GLfloat *_vertices;
  int _numVertices;

  const GLfloat *_colors;
  int _numColors;

  const GLubyte *_indices;
  int _numIndices;

  glm::vec3 _center;
};

}  // namespace kube

#endif  // _MODEL_HPP
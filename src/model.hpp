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

namespace kube {

class Model {
 public:
  struct Options {
    glm::vec3 center;
    const GLfloat *colors;
    const GLubyte *indices;
    int numColors;
    int numIndices;
    int numVertices;
    glm::mat4 rotation = glm::mat4(1.f);
    float scale = 1.f;
    const GLfloat *vertices;
  };

  Model() = delete;

  // Constructs a Model from the given Options.
  explicit Model(Options options) : _options(options) {}

  const glm::vec3 Center() { return _options.center; }

  const glm::mat4 Rotation() { return _options.rotation; }

  const GLfloat *Vertices() { return _options.vertices; }
  int NumVertices() { return _options.numVertices; }

  const GLfloat *Colors() { return _options.colors; }
  int NumColors() { return _options.numColors; }

  const GLubyte *Indices() { return _options.indices; }
  int NumIndices() { return _options.numIndices; }

  const GLfloat Scale() { return _options.scale; }

  void RotateRight(float t) {
    auto angle = glm::radians(ROTATIONS_PER_SEC) * t;
    _options.rotation = glm::rotate(_options.rotation, angle, -Y_AXIS);
  }

  void RotateLeft(float t) {
    auto angle = glm::radians(ROTATIONS_PER_SEC) * t;
    _options.rotation = glm::rotate(_options.rotation, angle, Y_AXIS);
  }

 private:
  Options _options;
};

}  // namespace kube

#endif  // _MODEL_HPP
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

namespace kube {

class Model {
public:
  Model() = delete;

  // Constructs a Model with the given center position.
  //
  // shader is used when drawing the model.
  Model(glm::vec3 center, VertexShader shader)
      : _center(center), _shader(shader) {}

  void Draw() { _shader.DrawModel(*this); }

private:
  glm::vec3 _center;
  VertexShader _shader;
};

} // namespace kube
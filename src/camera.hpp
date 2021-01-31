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

#include <glm/glm.hpp>

namespace kube {

class Camera {
 public:
  Camera(glm::vec3 position, glm::vec3 target, float fov = glm::radians(45.0f),
         float aspectRatio = 4.0f / 3.0f, float near = 0.1f, float far = 100.f,
         glm::vec3 up = glm::vec3(0, 1, 0))
      : _projection(glm::perspective(fov, aspectRatio, near, far)),
        _view(glm::lookAt(position, target, up)) {}

  glm::mat4 Project(glm::mat4 model) { return _projection * _view * model; }

 private:
  glm::mat4 _projection;
  glm::mat4 _view;
};

};  // namespace kube
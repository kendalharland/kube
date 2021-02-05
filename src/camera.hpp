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

#ifndef _CAMERA_HPP
#define _CAMERA_HPP

#include <glm/glm.hpp>

#define GLM_FORCE_RADIANS 1

namespace kube {

class Camera {
 public:
  Camera(glm::vec3 position, glm::vec3 target, float fov = glm::radians(45.0f),
         float aspectRatio = 4.0f / 3.0f, float near = 0.1f, float far = 100.f,
         glm::vec3 up = glm::vec3(0, 1, 0), float zoomSpeed = 4.f)
      : _aspectRatio(aspectRatio),
        _far(far),
        _fov(fov),
        _near(near),
        _position(position),
        _target(target),
        _up(up),
        _zoomSpeed(glm::radians(zoomSpeed)) {}

  glm::mat4 MVP(glm::mat4 model) {
    auto projection = glm::perspective(_fov, _aspectRatio, _near, _far);
    auto view = glm::lookAt(_position, _target, _up);
    return projection * view * model;
  }

  // TODO(kjharland): Clamp the camera between [0, 2π] so it doesn't flip.
  void Zoom(bool in) { _fov += in ? -_zoomSpeed : _zoomSpeed; }

 private:
  float _aspectRatio;
  float _far;
  float _fov;
  float _near;
  float _zoomSpeed;
  glm::vec3 _position;
  glm::vec3 _target;
  glm::vec3 _up;
};

};  // namespace kube

#endif  // _CAMERA_HPP
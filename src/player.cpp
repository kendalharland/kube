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

#include <GLFW/glfw3.h>
#include <player.h>

#include <animation.hpp>
#include <glm/glm.hpp>
#include <model.hpp>

namespace kube {

//////////////////////////////////////////////////////////////////////////////////////////
// Player
//

Player::Player(DeprecatedModel *model, PlayerState *state)
    : _model(model), _state(state) {}

void Player::SetModelRotation(double rotation, glm::vec3 axis) {
  _model->SetRotation(rotation, axis);
}

void Player::HandleInput(GLFWwindow *window) { _state->HandleInput(window); }
void Player::Update(double dt) { _state = _state->Update(dt, this); }

//////////////////////////////////////////////////////////////////////////////////////////
// PlayerIdleState
//

PlayerIdleState::PlayerIdleState() {}

void PlayerIdleState::HandleInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    // TODO: The tween should start from the cube's current rotation.
    _next = new PlayerRollingState(RotateAnimation(
        AnimationState(), DoubleTween(glm::radians(0.f), glm::radians(90.f)),
        (Curve &)LinearCurve, X_AXIS));
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    _next = new PlayerRollingState(RotateAnimation(
        AnimationState(), DoubleTween(glm::radians(0.f), glm::radians(90.f)),
        (Curve &)LinearCurve, -X_AXIS));
  }

  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    _next = new PlayerRollingState(RotateAnimation(
        AnimationState(), DoubleTween(glm::radians(0.f), glm::radians(90.f)),
        (Curve &)LinearCurve, Z_AXIS));
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    _next = new PlayerRollingState(RotateAnimation(
        AnimationState(), DoubleTween(glm::radians(0.f), glm::radians(90.f)),
        (Curve &)LinearCurve, -Z_AXIS));
  }
}

PlayerState *PlayerIdleState::Update(double dt, Player *player) {
  return _next;
}

//////////////////////////////////////////////////////////////////////////////////////////
// PlayerRollingState
//

PlayerRollingState::PlayerRollingState(RotateAnimation animation)
    : _animation(animation) {}

void PlayerRollingState::HandleInput(GLFWwindow *window) {
  glfwGetKey(window, 0);
}

PlayerState *PlayerRollingState::Update(double dt, Player *player) {
  double rotation = _animation.Update(dt);
  player->SetModelRotation(rotation, _animation.Axis());
  if (_animation.IsComplete()) {
    return new PlayerIdleState();
  }
  return this;
};

} // namespace kube

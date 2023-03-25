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

#ifndef _PLAYER_HPP
#define _PLAYER_HPP

#include <animation.hpp>
#include <model.h>
#include <window.h>

#include <glm/glm.hpp>

namespace kube {

class PlayerState;

class Player {
public:
  Player(Model *model, PlayerState *state);

  void HandleInput(Window *window);
  void Update(double dt);
  void SetModelRotation(double rotation, glm::vec3 axis);

private:
  Model *_model;
  PlayerState *_state;
};

class PlayerState {
public:
  virtual void HandleInput(Window *window) = 0;
  virtual PlayerState *Update(double dt, Player *player) = 0;
};

class PlayerIdleState : public virtual PlayerState {
public:
  PlayerIdleState();
  void HandleInput(Window *window);
  PlayerState *Update(double dt, Player *player);

private:
  PlayerState *_next = this;
};

class PlayerRollingState : public virtual PlayerState {
public:
  PlayerRollingState(RotateAnimation animation);
  void HandleInput(Window *window);
  PlayerState *Update(double dt, Player *player);

private:
  RotateAnimation _animation;
};

} // namespace kube

#endif // _PLAYER_HPP
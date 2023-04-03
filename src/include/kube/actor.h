// Kube  :  An OpenGL based game engine.
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

#pragma once

#include <map>

#include <kube/animation.h>
#include <kube/input.h>
#include <kube/model.h>
#include <kube/window.h>

#include <glm/glm.hpp>

namespace kube {

class ActorState;

class Actor {
public:
  Actor() {}
  Actor(std::shared_ptr<Model> model, std::unique_ptr<ActorState> state)
      : model_(std::move(model)), state_(std::move(state)) {}

  void HandleInput(Window *window);
  void Update(double dt);
  std::shared_ptr<InputHandler> GetInputHandler();
  void SetInputHandler(std::shared_ptr<InputHandler> input_handler);
  std::shared_ptr<Model> GetModel();
  void SetModel(std::shared_ptr<Model> model);
  void SetState(std::unique_ptr<ActorState> state);

private:
  std::shared_ptr<InputHandler> input_handler_;
  std::shared_ptr<Model> model_;
  std::unique_ptr<ActorState> state_;
};

class ActorCommand {
public:
  virtual void Execute(Actor *actor) = 0;
};

class ActorState {
public:
  virtual ~ActorState(){};
  virtual std::shared_ptr<ActorCommand> HandleInput(Window *window) = 0;
  virtual void Update(double dt, Actor *actor) = 0;
};

} // namespace kube

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

#include <kube/actor.h>
#include <kube/animation.h>
#include <kube/model.h>

#include <glm/glm.hpp>

namespace kube {

void Actor::SetModel(std::shared_ptr<Model> model) { model_ = std::move(model); }
std::shared_ptr<Model> Actor::GetModel() { return model_; }

void Actor::SetState(std::unique_ptr<ActorState> state) { state_ = std::move(state); }

void Actor::SetInputHandler(std::shared_ptr<InputHandler> input_handler) {
  input_handler_ = input_handler;
}
std::shared_ptr<InputHandler> Actor::GetInputHandler() { return input_handler_; }

void Actor::HandleInput(Window *window) {
  auto command = state_->HandleInput(window);
  if (command != nullptr) {
    command->Execute(this);
  }
}
void Actor::Update(double dt) { state_->Update(dt, this); }

} // namespace kube

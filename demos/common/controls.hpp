
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

#include <memory>

#include <kube/actor.h>
#include <kube/animation.h>
#include <kube/input.h>

#define ANIMATION_SPEED 2.0
static const glm::mat4 IDENTITY_MAT4 = glm::mat4(1.f);
static const glm::vec3 Y_AXIS = glm::vec3(0.f, 1.f, 0.f);
static const glm::vec3 X_AXIS = glm::vec3(1.f, 0.f, 0.f);
static const glm::vec3 Z_AXIS = glm::vec3(0.f, 0.f, 1.f);

class RotatingState;

class IdleState final : public kube::ActorState {
public:
  IdleState();
  ~IdleState(){};

  std::shared_ptr<kube::ActorCommand> HandleInput(kube::Window *window);
  void Update(double, kube::Actor *);

private:
  std::unique_ptr<kube::ActorState> next_ = nullptr;
  kube::InputHandler input_handler_;
};

class RotatingState final : public kube::ActorState {
public:
  RotatingState(float start_angle, float stop_angle, glm::vec3 axis = Z_AXIS)
      : animation_(kube::Animation(ANIMATION_SPEED, kube::Sin)),
        tween_(kube::FloatTween(start_angle, stop_angle)), axis_(axis) {}

  ~RotatingState() {}

  std::shared_ptr<kube::ActorCommand> HandleInput(kube::Window *);
  void Update(double dt, kube::Actor *);

private:
  kube::Animation animation_;
  kube::FloatTween tween_;
  glm::vec3 axis_;
  double last_radians_ = glm::radians(0.0f);
};

class CommandRotate final : public kube::ActorCommand {
public:
  CommandRotate(glm::vec3 axis);

  void Execute(kube::Actor *actor);

private:
  glm::vec3 axis_;
};

class CommandIdle final : public kube::ActorCommand {
public:
  void Execute(kube::Actor *actor);
};

CommandRotate::CommandRotate(glm::vec3 axis) : axis_(axis) {}

void CommandRotate::Execute(kube::Actor *actor) {
  auto state = std::make_unique<RotatingState>(0, M_PI_2, axis_);
  actor->SetState(std::move(state));
}

void CommandIdle::Execute(kube::Actor *actor) {
  auto state = std::make_unique<IdleState>();
  actor->SetState(std::move(state));
}

enum Button {
  Idle = 0,
  RotateNegativeX = 1,
  RotatePositiveX = 2,
  RotatePositiveZ = 3,
  RotateNegativeZ = 4,
  RotatePositiveY = 5,
  RotateNegativeY = 6,
};

IdleState::IdleState() {
  input_handler_ = kube::InputHandler();
  input_handler_.AssignKey(kube::KEY_LEFT, Button::RotatePositiveX);
  input_handler_.AssignKey(kube::KEY_RIGHT, Button::RotateNegativeX);
  input_handler_.AssignKey(kube::KEY_UP, Button::RotatePositiveZ);
  input_handler_.AssignKey(kube::KEY_DOWN, Button::RotateNegativeZ);
  input_handler_.AssignKeyModifier(kube::KEY_LEFT_SHIFT);
  input_handler_.AssignModifiedKey(kube::KEY_RIGHT, Button::RotatePositiveY);
  input_handler_.AssignModifiedKey(kube::KEY_LEFT, Button::RotateNegativeY);
}

std::shared_ptr<kube::ActorCommand> IdleState::HandleInput(kube::Window *window) {
  switch (input_handler_.HandleInput(window)) {
  case Button::RotatePositiveZ:
    return std::make_shared<CommandRotate>(Z_AXIS);
  case Button::RotateNegativeZ:
    return std::make_shared<CommandRotate>(-Z_AXIS);
  case Button::RotatePositiveX:
    return std::make_shared<CommandRotate>(X_AXIS);
  case Button::RotateNegativeX:
    return std::make_shared<CommandRotate>(-X_AXIS);
  case Button::RotatePositiveY:
    return std::make_shared<CommandRotate>(Y_AXIS);
  case Button::RotateNegativeY:
    return std::make_shared<CommandRotate>(-Y_AXIS);
  }
  return nullptr;
}

void IdleState::Update(double dt, kube::Actor *control) {}

std::shared_ptr<kube::ActorCommand> RotatingState::HandleInput(kube::Window *window) {
  if (animation_.IsComplete()) {
    return std::make_shared<CommandIdle>();
  }
  return nullptr;
}

void RotatingState::Update(double dt, kube::Actor *control) {
  tween_.Update(animation_.Update(dt));
  auto radians = tween_.GetValue();
  auto delta_radians = radians - last_radians_;
  last_radians_ = radians;
  control->GetModel()->Rotate(delta_radians, axis_);
}

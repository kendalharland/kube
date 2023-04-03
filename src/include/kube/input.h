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

#include <kube/window.h>

#include <glm/glm.hpp>

namespace kube {

static const int KEY_RIGHT = GLFW_KEY_RIGHT;
static const int KEY_LEFT = GLFW_KEY_LEFT;
static const int KEY_DOWN = GLFW_KEY_DOWN;
static const int KEY_UP = GLFW_KEY_UP;
static const int KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
static const int KEY_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT;
static const int KEY_SPACE = GLFW_KEY_SPACE;

class InputHandler final {
public:
  typedef int button_t;

  button_t HandleInput(Window *window) {
    if (window->IsKeyPressed(key_modifier)) {
      for (const auto &key : modified_keymap) {
        if (window->IsKeyPressed(key.first)) {
          return key.second;
        }
      }
    } else {
      for (const auto &key : keymap) {
        if (window->IsKeyPressed(key.first)) {
          return key.second;
        }
      }
    }

    return -1;
  }

  void AssignKeyModifier(unsigned int key) { key_modifier = key; }
  void AssignKey(unsigned int key, button_t button) { keymap[key] = button; }
  void AssignModifiedKey(unsigned int key, button_t button) { modified_keymap[key] = button; }

private:
  unsigned int key_modifier;
  std::map<unsigned int, button_t> keymap;
  std::map<unsigned int, button_t> modified_keymap;
};

} // namespace kube
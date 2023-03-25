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

#include <kube/time.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace kube {

void Stopwatch::Start() { last_lap_ = glfwGetTime(); }

double Stopwatch::Lap() {
  auto now = glfwGetTime();
  auto elapsed = now - last_lap_;
  last_lap_ = now;
  return elapsed;
}

}; // namespace kube
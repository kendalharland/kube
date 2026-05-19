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

// ============================================================================
// Foreign bindings
// ============================================================================

// Global singleton representing the game window.
class Window {
  foreign static open(width, height, title)
}

// Camera object.
//
// Multiple cameras are supported.
foreign class Camera {
  foreign setPosition_(x, y, z)
  foreign setTarget_(x, y, z)

  // Set the player's perspective to this camera.
  // 
  // The perspective will be overridden the next time any camera instance calls
  // activate.
  foreign activate()

  construct new() {}

  // Set the camera's position.
  position=(vec) {
    setPosition_(vec.x, vec.y, vec.z)
  }

  target=(vec) {
    setTarget_(vec.x, vec.y, vec.z)
  }
}

foreign class Entity {
  foreign setModel_(model)
  foreign setPosition_(x, y, z)
  foreign setSpin_(x, y, z)
  foreign setShader_(value)

  construct new() {}
  
  model=(value) {
    setModel_(value)
  }
  
  shader=(value) {
    setShader_(value)
  }

  position=(vec) {
    setPosition_(vec.x, vec.y, vec.z)
  }

  spin=(vec) {
    setSpin_(vec.x, vec.y, vec.z)
  }
}

foreign class Model {
  construct new(id) {}
}

foreign class Shader {
  construct new(directory) {}
}

// ============================================================================
// Helpers
// ============================================================================

class Vec3 {
  construct new(x, y, z) {
    _x = x
    _y = y
    _z = z
  }

  x { _x }
  x=(value) { _x = value }

  y { _y }
  y=(value) { _y = value }

  z { _z }
  z=(value) { _z = value }
}

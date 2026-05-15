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

class Game {
    foreign static run()
}

class Window {
    foreign static open(width, height, title)
}

foreign class Camera {
  foreign setPosition_(x, y, z)
  foreign setActive()

  construct new() {}

  position=(vec) {
    setPosition_(vec.x, vec.y, vec.z)
  }
}

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

foreign class Entity {
    foreign setModel_(model)
    foreign setPosition_(x, y, z)
    foreign setSpin_(x, y, z)

    construct new() {}
    
    model=(value) {
        setModel_(value)
    }
    
    position=(vec) {
        setPosition_(vec.x, vec.y, vec.z)
    }

    spin=(vec) {
        setSpin_(vec.x, vec.y, vec.z)
    }
}

foreign class Model {
    construct new(identifier) {}
}

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

import "kube" for Camera, Entity, Model, Window, Vec3
import "random" for Random


// ============================================================================
// Scene setup
// ============================================================================


class Game {
    entities { _entities }
    entities=(value) { _entities = value }

    camera { _camera }
    camera=(value) { _camera = value }

    construct new() {
        // Open the window
        var title = "Rotating Kube"
        var width = 1600
        var height = 1200
        Window.open(width, height, title)

        // Initialize the camera
        var camera = Camera.new()
        camera.position = Vec3.new(100, 100, 100)
        camera.setActive()

        // Create some entities.
        var entities = []
        var random = Random.new(123456789)

        for (i in 1..4000) {
            var entity = Entity.new()
            entity.model = Model.new("@cube")
            entity.position = Vec3.new(
                random.float(-100, 100),
                random.float(-100, 100),
                random.float(-100, 100)
            )
            entity.spin = Vec3.new(
                random.float(-1, 1),
                random.float(-1, 1),
                random.float(-1, 1)
            )
            entities.add(entity)
        }

        this.camera = camera
        this.entities = entities
    }

    update(elapsedSecs) {
    }
}
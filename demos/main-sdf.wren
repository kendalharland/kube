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

import "kube" for Camera, Entity, Model, Window, Vec3, Shader
import "random" for Random
import "models" for MicroRecon

class Game {
    entities { _entities }
    entities=(value) { _entities = value }

    camera { _camera }
    camera=(value) { _camera = value }

    time { _time }
    time=(value) { _time = value }

    construct new() {
        // Open the window
        var title = "Rotating Kube"
        var width = 1600
        var height = 1200
        Window.open(width, height, title)

        // Initialize the camera
        var camera = Camera.new()
        camera.position = Vec3.new(3, 3, 3)
        camera.setActive()

        var shader = Shader.new("demos/shaders/sdf")

        var entities = [] 
        for (i in 1..3) {
            var entity = Entity.new()
            entity.model = Model.new(MicroRecon)
            entity.position = Vec3.new(i,0,0)
            entity.spin = Vec3.new(1+i,-1,1)
            entity.shader = shader
            entities.add(entity)
        }
        {
            var entity = Entity.new()
            entity.model = Model.new("@quad")
            entity.position = Vec3.new(3, 0, 0)
            entity.shader = shader
            entities.add(entity)
        }

        this.time = 0
        this.camera = camera
        this.entities = entities
    }

    update(elapsedSecs) {
        this.time = this.time + elapsedSecs
        var amplitude = 3
        var frequency = 0.15
        var angle = 2.0 * Num.pi * frequency * this.time
        var offset = angle.sin * amplitude
        camera.position = Vec3.new(10 + offset, 10 + offset, 10 + offset)
    }
}
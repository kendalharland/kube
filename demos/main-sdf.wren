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
        Window.open(1600, 1200, "kube")

        // Initialize the camera
        var camera = Camera.new()
        camera.position = Vec3.new(0, -20, 0)
        camera.target = Vec3.new(0, 0, 0)
        camera.activate()

        var sdfShader = Shader.new("demos/shaders/sdf")
        var shipShader = Shader.new("demos/shaders/diffuse")
        var cubeShader = Shader.new("demos/shaders/simple_color")

        var entities = [] 
        {
            var entity = Entity.new()
            entity.model = Model.new("@quad")
            entity.position = Vec3.new(3, 0, 0)
            entity.shader = sdfShader
            entities.add(entity)
        }
        this.time = 0
        this.camera = camera
        this.entities = entities
    }

    update(elapsedSecs) {}
}
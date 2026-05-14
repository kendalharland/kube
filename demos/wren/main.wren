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

import "kube" for Game, Entity, Model, Window

// ============================================================================
// Window setup
// ============================================================================

var title = "Rotating Kube"
var width = 1000
var height = 800

Window.open(width, height, title)

// ============================================================================
// Scene setup
// ============================================================================

var entity1 = Entity.new()
var model1 = Model.new("@cube")
entity1.setModel(model1)
entity1.setPosition(-1, 0, 0)

var entity2 = Entity.new()
var model2 = Model.new("@cube")
entity2.setModel(model2)
entity2.setPosition(1, 0, 0)

// ============================================================================
// Main loop
// ============================================================================

Game.run()
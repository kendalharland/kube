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

// TODO: Consider creating a separate shader for the cube
// and for imported models which actually have textures.

#version 330 core

layout(location = 0) in vec3 position;  // Vertex position in object space.
layout(location = 1) in vec3 color;

out vec3 fragment_color;    // Base color.

uniform mat4 MVP; // Model-View-Projection matrix.

void main() {
	gl_Position =  MVP * vec4(position, 1);

	fragment_color = color;
}

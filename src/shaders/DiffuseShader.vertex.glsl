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
layout(location = 2) in vec3 normal;    // Vertex normal in object space.
layout(location = 3) in vec2 tex_coord; 
layout(location = 4) in vec2 tangent;   // Vertex tangent in object space.
layout(location = 5) in vec2 bitangent; // Vertex bitangent in object space.

out vec4 fragment_position; // Position in world space.
out vec4 fragment_normal;   // Surface normal in world space.
out vec3 fragment_color;    // Base color.
out vec2 fragment_tex_coord;
out vec4 light_direction;   // Direction light reflects off surface onto camera.

uniform mat4 MVP; // Model-View-Projection matrix.
uniform vec4 light_position; // Light position in world space.

void main() {
	gl_Position =  MVP * vec4(position, 1);
	
	fragment_position = vec4(position, 1);
	fragment_normal = vec4(normal, 1);
	fragment_normal = normalize(fragment_normal);

	fragment_color = color;
    fragment_tex_coord = tex_coord;
	
	light_direction = light_position - vec4(position, 1);
	light_direction = normalize(light_direction);
}

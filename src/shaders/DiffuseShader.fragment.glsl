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

#version 330 core

out vec4 FragColor;

// Interpolated values from the vertex shaders
in vec3 fragment_color;
in vec2 fragment_tex_coord;
in vec4 fragment_position; // Position in world space.
in vec4 fragment_normal; // Surface normal in world space.
in vec4 light_direction; // Direction in which light hits this surface.

uniform vec4 light_color; // Light's diffuse and specular contribution.

// For now, we only support one type of each sampler.
uniform sampler2D sampler_ambient;
uniform sampler2D sampler_diffuse;
uniform sampler2D sampler_specular;

void main(){
	vec4 tex_ambient = texture(sampler_ambient, fragment_tex_coord);
	vec4 tex_diffuse = texture(sampler_diffuse, fragment_tex_coord);
	vec4 tex_specular = texture(sampler_specular, fragment_tex_coord);

	vec4 N = normalize(fragment_normal); 
	vec4 L = normalize(light_direction);
	vec4 diffuse_color = light_color * clamp(dot(N, L), 0.0, 1.0);
	FragColor = (tex_ambient + tex_specular + diffuse_color) * tex_diffuse;
}
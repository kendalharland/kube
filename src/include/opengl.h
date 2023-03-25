// Kube - A puzzle video game.
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

#ifndef _OPENGL_H
#define _OPENGL_H

#include <functional>

#include <mesh.h>
#include <singleton.h>

#include <GL/glew.h>

namespace kube {
namespace graphics {

void LoadMesh(Mesh &mesh);
void UnloadMesh(Mesh &mesh);
void DrawMesh(Mesh &mesh, Shader &shader, glm::mat4 mvp);

}; // namespace graphics
}; // namespace kube

#endif // _OPENGL_H
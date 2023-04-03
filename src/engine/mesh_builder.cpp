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

#include <kube/mesh.h>

#include "mesh_builder.h"

void MeshBuilder::AddVertex(Vertex &&vertex) { vertices_.push_back(std::move(vertex)); }

void MeshBuilder::AddIndex(unsigned int index) { indices_.push_back(std::move(index)); }

void MeshBuilder::AddTexture(texture_ptr texture) { textures_.push_back(std::move(texture)); }

void MeshBuilder::SetMaterial(Material &&material) { material_ = std::move(material); }

std::unique_ptr<Mesh> MeshBuilder::Build() {
  auto vertex_array = std::make_unique<VertexArray>(std::move(vertices_), std::move(indices_));
  return std::make_unique<Mesh>(std::move(vertex_array), std::move(textures_),
                                std::move(material_));
}

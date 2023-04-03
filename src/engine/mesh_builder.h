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

#pragma once

#include <memory>

#include <kube/mesh.h>

using namespace kube::graphics;

class MeshBuilder {
public:
  void AddVertex(Vertex &&vertex);
  void AddIndex(unsigned int index);
  void AddTexture(texture_ptr texture);
  void SetMaterial(Material &&material);
  std::unique_ptr<Mesh> Build();

private:
  Material material_;
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  std::vector<texture_ptr> textures_;
};

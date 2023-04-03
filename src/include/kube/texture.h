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
#include <string>

#include <kube/internal/constructors.h>
#include <kube/shader.h>

namespace kube {
namespace graphics {

class Texture final {
public:
  UNCOPYABLE(Texture);
  Texture(std::string filename, std::string type);
  ~Texture() {}

  void Load();
  void Unload();

  std::string GetFilename() const;
  void Bind(std::shared_ptr<Shader> shader, unsigned int index) const;

private:
  std::string filename_;
  std::string type_;
  unsigned int ID_;
};

typedef std::unique_ptr<Texture> texture_ptr;

} // namespace graphics
} // namespace kube
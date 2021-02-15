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

#ifndef _LEVEL_HPP
#define _LEVEL_HPP

#define TILEMAP_WIDTH 10
#define TILEMAP_HEIGHT 10

namespace kube {

enum TileColor {
  Blue,
  Green,
  Red,
  Yellow,
  Orange,
  Purple,
  Silver,
};

struct Tile {
  int x;
  int y;
  TileColor color;
  bool isActive;
};

class Level {
 public:
  struct Data {
    Tile tiles[TILEMAP_WIDTH][TILEMAP_HEIGHT];
  };

  Level() = delete;
  Level(Data data) : _data(data) {}

 private:
  Data _data;
};

};  // namespace kube

#endif  // _LEVEL_HPP
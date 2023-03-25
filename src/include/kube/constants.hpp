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

#ifndef _CONSTANTS_HPP
#define _CONSTANTS_HPP

#include <glm/glm.hpp>

#define TITLE "Kube"

// Camera is at (4,3,-3), in World Space
// and looks at the origin

#define CAMERA_POS glm::vec3(-15, 10, -25)
#define CAMERA_TARGET glm::vec3()

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define TARGET_FPS 60

#define CUBE_SIZE 1.f

#define IDENTITY_MAT4 glm::mat4(1.f)
#define ORIGIN glm::vec3(0.0f)

#endif // _CONSTANTS_HPP
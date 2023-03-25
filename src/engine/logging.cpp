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

#include <iostream>
#include <string>

#include <kube/logging.h>

namespace kube {

void kube_log_internal(const char *file, int line, LogLevel level, std::string_view message) {
  std::string lev;
  switch (level) {
  case LogLevel::info:
    lev = "INFO";
    break;
  case LogLevel::warn:
    lev = "WARNING";
    break;
  case LogLevel::error:
    lev = "ERROR";
    break;
  }

  std::clog << "[" << lev << "][" << file << ":" << line << "] " << message << std::endl;
}

} // namespace kube

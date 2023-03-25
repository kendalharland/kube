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

#ifndef _LOGGING_H
#define _LOGGING_H

#include <iostream>
#include <string>

using namespace std;

namespace kube {

enum LogLevel {
  info,
  warn,
  error,
};

// clang-format off
#define KUBE_LOG(level, message) kube_log_internal(__FILE__, __LINE__, level, message)
#define KUBE_INFO(message) KUBE_LOG(kube::LogLevel::info, message)
#define KUBE_WARN(message) KUBE_LOG(kube::LogLevel::warn, message)
#define KUBE_ERROR(message) KUBE_LOG(kube::LogLevel::error, message)
// clang-format on

void kube_log_internal(const char *file, int line, LogLevel level, std::string_view message);

} // namespace kube

#endif // _LOGGING_H
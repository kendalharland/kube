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

#include <iostream>

#include <kube/singleton.h>

namespace kube {

enum LogLevel {
  Debug,
  Info,
  Warn,
  Error,
};

#define KUBE_LOG(level) Logger().Log(__FILE__, __LINE__, level)
#define KUBE_DEBUG KUBE_LOG(kube::LogLevel::Debug)
#define KUBE_INFO KUBE_LOG(kube::LogLevel::Info)
#define KUBE_WARN KUBE_LOG(kube::LogLevel::Warn)
#define KUBE_ERROR KUBE_LOG(kube::LogLevel::Error)

class Logger final {

public:
  static void SetLogLevel(LogLevel);

  Logger(std::ostream &out = std::clog) : out_(out) {}
  Logger(Logger &) = delete;
  Logger(Logger &&) = delete;
  ~Logger();

  std::ostream &Log(const char *file, int line, LogLevel level);

private:
  std::ostream &out_;
  bool newline_on_destruct_ = false;

  static LogLevel global_log_level_;
};

} // namespace kube

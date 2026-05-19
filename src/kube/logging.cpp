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

#include <iostream>

#include <kube/logging.h>

namespace {

class NullBuffer final : public std::streambuf, public kube::Singleton<NullBuffer> {
private:
  SINGLETON(NullBuffer) {}

public:
  int overflow(int c) override { return c; }
};

class NullStream final : public std::ostream, public kube::Singleton<NullStream> {
public:
  SINGLETON(NullStream) : std::ostream(NullBuffer::GetInstance()) {}
};

} // namespace

namespace kube {

LogLevel Logger::global_log_level_ = LogLevel::Debug;

void Logger::SetLogLevel(LogLevel level) { global_log_level_ = level; }

Logger::~Logger() {
  if (newline_on_destruct_) {
    out_ << std::endl;
  }
}

std::ostream &Logger::Log(const char *file, int line, LogLevel level) {
  if (level < global_log_level_) {
    auto null_stream = NullStream::GetInstance();
    return *null_stream;
  }

  newline_on_destruct_ = true;

  std::string prefix;
  switch (level) {
  case LogLevel::Debug:
    prefix = "DEBUG";
    break;
  case LogLevel::Info:
    prefix = "INFO";
    break;
  case LogLevel::Warn:
    prefix = "WARNING";
    break;
  case LogLevel::Error:
    prefix = "ERROR";
    break;
  }

  out_ << "[" << prefix << "][" << file << ":" << line << "] ";
  return out_;
}

} // namespace kube

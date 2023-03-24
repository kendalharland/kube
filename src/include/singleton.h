
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

#ifndef _SINGLETON_H
#define _SINGLETON_H

namespace kube {

#define SINGLETON(IMPL)           \
  friend Singleton<IMPL>;         \
  static IMPL* CreateInstance() { \
    return new IMPL();            \
  }                               \
  IMPL()

template <typename Impl>
class Singleton {
  friend Singleton<Impl>;

 public:
  static Impl* GetInstance() {
    if (instance_ == nullptr) {
      instance_ = Impl::CreateInstance();
    }
    return instance_;
  }

 private:
  static Impl* instance_;
};

template <typename Impl>
Impl* Singleton<Impl>::instance_ = nullptr;

}  // namespace kube

#endif  // _SINGLETON_H

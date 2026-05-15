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

#include <stdio.h>
#include <stdlib.h>

#include <kube/camera.h>
#include <kube/actor.h>
#include <kube/logging.h>
#include <kube/mesh.h>
#include <kube/model.h>
#include <kube/fs.h>
#include <kube/time.h>
#include <kube/window.h>
#include <kube/wren.h>

#include "controls.hpp"
#include "wren.hpp"

int main(void) {
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.writeFn = &wrenWriteFn;
  config.errorFn = &wrenErrorFn;
  config.bindForeignMethodFn = &wrenBindForeignMethod;
  config.bindForeignClassFn = &wrenBindForeignClass;
  config.loadModuleFn = &wrenLoadModule;

  WrenVM* vm = wrenNewVM(&config);

  wrenInitGame();
  auto source = kube::fs::readFile("demos/wren/main.wren");
  
  WrenInterpretResult result = wrenInterpret(vm, WREN_GAME_MODULE, source.c_str());

  switch (result) {
  case WREN_RESULT_COMPILE_ERROR:
    { printf("Compile Error!\n"); } break;
  case WREN_RESULT_RUNTIME_ERROR:
    { printf("Runtime Error!\n"); } break;
  case WREN_RESULT_SUCCESS:
    { printf("Success!\n"); } break;
  }

  wrenRunGame(vm);
  wrenFreeVM(vm);
  return 0;
}

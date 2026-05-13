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
#include <kube/shapes.cpp>
#include <kube/time.h>
#include <kube/window.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "controls.hpp"
#include "wren.hpp"

std::string ReadFile(std::string filename) {
  std::ifstream file(filename, std::ios::in);
  if (!file.is_open()) {
    throw std::runtime_error("failed to open shader file " + filename);
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}


void wrenWriteFn(WrenVM* vm, const char* text) {
  printf("%s", text);
}

void runGameLoop(WrenVM* vm);
void wrenErrorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
  switch (errorType)
  {
    case WREN_ERROR_COMPILE:
    {
      printf("[%s line %d] [Error] %s\n", module, line, msg);
    } break;
    case WREN_ERROR_STACK_TRACE:
    {
      printf("[%s line %d] in %s\n", module, line, msg);
    } break;
    case WREN_ERROR_RUNTIME:
    {
      printf("[Runtime Error] %s\n", msg);
    } break;
  }
}

bool streq(std::string a, std::string b) {
  return a.compare(b) == 0;
}


void runGameLoop(WrenVM* vm) {
  using namespace kube::graphics;
  
  auto window = kube::Window::GetInstance();

  shader_ptr shader = Shader::SimpleColorShader("src/shaders");
  
  kube::Actor actor;
  actor.SetModel(kube::CreateCubeModel());
  actor.SetState(std::make_unique<IdleState>());

  kube::Stopwatch stopwatch;
  stopwatch.Start();

  do {
    auto dt = stopwatch.Lap();

    actor.HandleInput(window);
    actor.Update(dt);
    window->Clear();
    actor.GetModel()->Draw(window->GetCamera(), shader);
    window->Update();
  } while (!window->ShouldClose());

  window->Close();
}

void openWindow(int width, int height, char* title) {
  auto window = kube::Window::GetInstance();
  window->Open(width, height, title);
  window->SetCamera(std::make_unique<kube::Camera>());
}

void wrenOpenWindow(WrenVM* vm) {
  auto width = (int)wrenGetSlotDouble(vm, 1);
  auto height = (int)wrenGetSlotDouble(vm, 2);
  auto title = (char*)wrenGetSlotString(vm, 3);
  openWindow(width, height, title);
}

WrenForeignMethodFn wrenBindForeignMethod(
    WrenVM* vm,
    const char* module,
    const char* className,
    bool isStatic,
    const char* signature
) {
  // Assume module is main
  if (streq("Window", className)) {
    if (isStatic) {
      if (streq("open(_,_,_)", signature)) return wrenOpenWindow;
    }
  }
  return runGameLoop;
}


int initializeWren() {
  WrenConfiguration config;
  wrenInitConfiguration(&config);
  config.writeFn = &wrenWriteFn;
  config.errorFn = &wrenErrorFn;
  config.bindForeignMethodFn = &wrenBindForeignMethod;

  WrenVM* vm = wrenNewVM(&config);

  const char* module = "main";
  auto source = ReadFile("demos/wren/main.wren");
  
  WrenInterpretResult result = wrenInterpret(vm, module, source.c_str());

  switch (result) {
  case WREN_RESULT_COMPILE_ERROR:
    { printf("Compile Error!\n"); } break;
  case WREN_RESULT_RUNTIME_ERROR:
    { printf("Runtime Error!\n"); } break;
  case WREN_RESULT_SUCCESS:
    { printf("Success!\n"); } break;
  }
  wrenFreeVM(vm);
  return 0;
}

int main(void) {
  return initializeWren();
}

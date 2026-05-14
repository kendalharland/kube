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

#include <stdio.h>
#include <stdlib.h>

#include <kube/actor.h>
#include <kube/camera.h>
#include <kube/entity.h>
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

#include "wren.hpp"

// -- Constants

#define MAX_MODELS MAX_ENTITIES
typedef int ModelID;

// TODO: Game instance.
auto entities = kube::EntityStore();

// -- Helpers

bool streq(std::string a, std::string b) { return a.compare(b) == 0; }

// -- Engine handlers

void openWindow(int width, int height, char *title) {
  auto window = kube::Window::GetInstance();
  window->Open(width, height, title);
  window->SetCamera(std::make_unique<kube::Camera>());
}

// -- Wren handlers

typedef struct Entity {
  EntityID id;
} Entity;

WrenLoadModuleResult wrenLoadModule(WrenVM *vm, const char *name) {
  WrenLoadModuleResult result = {0};
  char fullname[32];
  if (snprintf(fullname, 32, "demos/wren/%s.wren", name) < 0) {
    // panic.
  }

  auto source = kube::fs::readFile(fullname);
  char *c_source = new char[source.length() + 1];
  std::strcpy(c_source, source.c_str());
  result.source = c_source;
  return result;
}

void wrenRunGame(WrenVM *vm) {
  using namespace kube::graphics;

  auto window = kube::Window::GetInstance();

  shader_ptr shader = Shader::SimpleColorShader("src/shaders");

  // TODO(controls, state)
  // kube::Stopwatch stopwatch;
  // stopwatch.Start();

  do {
    // auto dt = stopwatch.Lap();

    window->Clear();

    // Draw entities
    for (auto entity : entities.GetEntitiesWithModelComponent()) {
      auto component = entities.GetModelComponent(entity.id);
      component->model.Draw(window->GetCamera(), shader);
    }

    window->Update();
  } while (!window->ShouldClose());

  window->Close();
}

void wrenWriteFn(WrenVM *vm, const char *text) { printf("%s", text); }

void wrenErrorFn(WrenVM *vm, WrenErrorType errorType, const char *module, const int line,
                 const char *msg) {
  switch (errorType) {
  case WREN_ERROR_COMPILE: {
    printf("[%s line %d] [Error] %s\n", module, line, msg);
  } break;
  case WREN_ERROR_STACK_TRACE: {
    printf("[%s line %d] in %s\n", module, line, msg);
  } break;
  case WREN_ERROR_RUNTIME: {
    printf("[Runtime Error] %s\n", msg);
  } break;
  }
}

void wrenOpenWindow(WrenVM *vm) {
  auto width = (int)wrenGetSlotDouble(vm, 1);
  auto height = (int)wrenGetSlotDouble(vm, 2);
  auto title = (char *)wrenGetSlotString(vm, 3);
  openWindow(width, height, title);
}

void wrenEntityAlloc(WrenVM *vm) {
  auto entity = (Entity *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(Entity));
  entity->id = entities.CreateEntity();
}

void wrenEntityDealloc(void *entity) {
  // TODO: Call the destructor explicitly; Wren will free the raw memory.
}

static void wrenEntitySetModel(WrenVM *vm) {
  auto entity = (Entity *)(wrenGetSlotForeign(vm, 0));

  auto model = wrenGetSlotString(vm, 1);
  if (streq(model, "cube")) {
    auto cube = kube::CreateCubeModel();
    auto component = kube::ModelComponent{};
    component.model = std::move(cube);
    entities.AddModelComponent(entity->id, std::move(component));
  }
}

// -- Binding table lookup

using NativeFn = void (*)(WrenVM *vm); // adjust as needed

// Helper to build a unique key "module|class|signature"
inline std::string makeKey(std::string_view module, std::string_view className,
                           std::string_view signature, bool isStatic) {
  // Encode static-ness into the key so static/instance can differ
  char staticFlag = isStatic ? 'S' : 'I';

  std::string key;
  key.reserve(module.size() + className.size() + signature.size() + 4);
  key.append(module);
  key.push_back('|');
  key.append(className);
  key.push_back('|');
  key.append(signature);
  key.push_back('|');
  key.push_back(staticFlag);
  return key;
}

using BindingTable = std::unordered_map<std::string, NativeFn>;

BindingTable &getBindingTable() {
  static BindingTable table = {{makeKey("kube", "Game", "run()", 1), &wrenRunGame},
                               {makeKey("kube", "Window", "open(_,_,_)", 1), &wrenOpenWindow},
                               {makeKey("kube", "Entity", "setModel(_)", 0), &wrenEntitySetModel}};
  return table;
}

WrenForeignClassMethods wrenBindForeignClass(WrenVM *vm, const char *module,
                                             const char *className) {
  KUBE_DEBUG << "foreign_class:" << module << "." << className;

  WrenForeignClassMethods methods{};

  if (streq(module, "kube") && streq(className, "Entity")) {
    methods.allocate = wrenEntityAlloc;
    methods.finalize = wrenEntityDealloc;
  } else if (streq(module, "kube") && streq(className, "Model")) {
    methods.allocate = wrenEntityAlloc;
    methods.finalize = wrenEntityDealloc;
  }

  return methods;
}

WrenForeignMethodFn wrenBindForeignMethod(WrenVM *vm, const char *module, const char *className,
                                          bool isStatic, const char *signature) {
  KUBE_DEBUG << "foreign_call:" << module << "." << className << "." << signature;

  auto &table = getBindingTable();
  std::string key = makeKey(module, className, signature, isStatic);

  auto it = table.find(key);
  if (it != table.end()) {
    return it->second;
  }
  return nullptr;
}

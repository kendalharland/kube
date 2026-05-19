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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#include <kube/camera.h>
#include <kube/entity.h>
#include <kube/logging.h>
#include <kube/scripting_engine.h>
#include <kube/shapes.h>
#include <kube/time.h>
#include <kube/window.h>

#include "wren.hpp"

#define WREN_GAME_MODULE "main"
#define WREN_GAME_CLASSNAME "Game"

typedef struct CameraHandle {
  CameraID id;
} CameraHandle;

typedef struct EntityHandle {
  EntityID id;
} EntityHandle;

typedef struct ModelHandle {
  kube::model value;
} ModelHandle;

typedef struct ShaderHandle {
  std::shared_ptr<kube::shader> value;
} ShaderHandle;

kube::Game *game = nullptr;
std::string mainModulePath;
WrenHandle *userGameInstance = nullptr;
WrenHandle *userGameUpdateFn = nullptr;

void wrenInitGame(std::string mainModulePath) {
  ::mainModulePath = mainModulePath;
  if (game == nullptr) {
    game = kube::newGame();
  }
}

void wrenGameUpdate(WrenVM *vm, double elapsedSeconds) {
  wrenSetSlotHandle(vm, 0, userGameInstance);
  wrenSetSlotDouble(vm, 1, elapsedSeconds);
  WrenInterpretResult result = wrenCall(vm, userGameUpdateFn);

  switch (result) {
  case WREN_RESULT_COMPILE_ERROR: {
    printf("Compile Error!\n");
  } break;
  case WREN_RESULT_RUNTIME_ERROR: {
    printf("Runtime Error!\n");
  } break;
  case WREN_RESULT_SUCCESS:
    break;
  }
}

void wrenRunGame(WrenVM *vm) {
  if (game->running) {
    return;
  }

  // Find the user's Game class.
  wrenEnsureSlots(vm, 1);
  wrenGetVariable(vm, WREN_GAME_MODULE, WREN_GAME_CLASSNAME, 0);
  auto userGameClass = wrenGetSlotHandle(vm, 0);

  // Call Game.new() to allocate an instance
  wrenSetSlotHandle(vm, 0, userGameClass);
  WrenHandle *ctor = wrenMakeCallHandle(vm, "new()");
  wrenCall(vm, ctor);

  // Slot 0 now contains a Game instance
  userGameInstance = wrenGetSlotHandle(vm, 0);
  wrenReleaseHandle(vm, userGameClass);
  wrenReleaseHandle(vm, ctor);

  // Create a handle to call the game's update function.
  userGameUpdateFn = wrenMakeCallHandle(vm, "update(_)");

  kube::gameLoop(game, [vm](double elapsedSeconds) { wrenGameUpdate(vm, elapsedSeconds); });
}

WrenLoadModuleResult wrenLoadModule(WrenVM *vm, const char *name) {
  WrenLoadModuleResult result = {0};

  std::vector<std::filesystem::path> searchPaths = {
      std::format("{}/{}.wren", mainModulePath, name),
      std::format("./{}.wren", name),
      // TODO: stdlib when run from the project root. embed this file.
      std::format("src/wren/{}.wren", name),
      std::format("/usr/local/include/{}", name),
      std::format("/usr/include/{}", name),
  };

  std::filesystem::path filename;

  bool found = false;

  for (auto path : searchPaths) {
    filename = path.lexically_normal();
    KUBE_INFO << "searching for module at " << filename;
    if (std::filesystem::exists(filename)) {
      found = true;
      break;
    }
  }

  if (!found) {
    throw std::invalid_argument("module not found: " + std::string(name));
  }

  auto source = kube::fs::readFile(filename);
  char *c_source = new char[source.length() + 1];
  std::strcpy(c_source, source.c_str());
  result.source = c_source;
  return result;
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
  kube::openWindow(width, height, title);
}

// ============================================================================
// Allocators
// ============================================================================

void wrenCameraAlloc(WrenVM *vm) {
  auto camera = (CameraHandle *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(CameraHandle));
  camera->id = kube::createCamera(game);
}

void wrenCameraDealloc(void *camera) { delete (CameraHandle *)camera; }

void wrenEntityAlloc(WrenVM *vm) {
  auto entity = (EntityHandle *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(EntityHandle));
  entity->id = kube::createEntity(game);
}

void wrenEntityDealloc(void *entity) { delete (EntityHandle *)entity; }

void wrenModelAlloc(WrenVM *vm) {
  auto model = (ModelHandle *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(ModelHandle));
  auto id = wrenGetSlotString(vm, 1);
  model->value = kube::createModel(id);
}

void wrenModelDealloc(void *handle) {
  auto model = (ModelHandle *)handle;
  kube::modelUnload(model->value);
  delete model;
}

void wrenShaderAlloc(WrenVM *vm) {
  auto shader = (ShaderHandle *)wrenSetSlotNewForeign(vm, 0, 0, sizeof(ShaderHandle));
  auto path = wrenGetSlotString(vm, 1);
  shader->value = std::make_shared<kube::shader>(kube::loadShader(path));
}

void wrenShaderDealloc(void *handle) {
  auto shader = (ShaderHandle *)handle;
  kube::shaderUnload(*shader->value);
  delete shader;
}

// ============================================================================
// Methods
// ============================================================================

static void wrenCameraSetActive(WrenVM *vm) {
  auto camera = (CameraHandle *)(wrenGetSlotForeign(vm, 0));
  kube::cameraSetActive(game, camera->id);
}

static void wrenCameraSetPosition(WrenVM *vm) {
  auto camera = (CameraHandle *)(wrenGetSlotForeign(vm, 0));
  auto x = wrenGetSlotDouble(vm, 1);
  auto y = wrenGetSlotDouble(vm, 2);
  auto z = wrenGetSlotDouble(vm, 3);
  kube::entitySetPosition(game, camera->id, glm::vec3(x, y, z));
}

static void wrenEntitySetModel(WrenVM *vm) {
  auto entity = (EntityHandle *)(wrenGetSlotForeign(vm, 0));
  auto model = (ModelHandle *)wrenGetSlotForeign(vm, 1);
  kube::entitySetModel(game, entity->id, std::move(model->value));
}

static void wrenEntitySetPosition(WrenVM *vm) {
  auto entity = (EntityHandle *)(wrenGetSlotForeign(vm, 0));
  auto x = wrenGetSlotDouble(vm, 1);
  auto y = wrenGetSlotDouble(vm, 2);
  auto z = wrenGetSlotDouble(vm, 3);
  kube::entitySetPosition(game, entity->id, glm::vec3(x, y, z));
}

static void wrenEntitySetSpin(WrenVM *vm) {
  auto entity = (EntityHandle *)(wrenGetSlotForeign(vm, 0));
  auto x = wrenGetSlotDouble(vm, 1);
  auto y = wrenGetSlotDouble(vm, 2);
  auto z = wrenGetSlotDouble(vm, 3);
  kube::entitySetSpin(game, entity->id, glm::vec3(x, y, z));
}

static void wrenEntitySetShader(WrenVM *vm) {
  auto entity = (EntityHandle *)(wrenGetSlotForeign(vm, 0));
  auto shader = (ShaderHandle *)(wrenGetSlotForeign(vm, 1));
  kube::entitySetShader(game, entity->id, shader->value);
}

// ============================================================================
// Foreign method and class bindings
// ============================================================================

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
  static BindingTable table = {
      {makeKey("kube", "Camera", "setActive()", 0), &wrenCameraSetActive},
      {makeKey("kube", "Camera", "setPosition_(_,_,_)", 0), &wrenCameraSetPosition},
      {makeKey("kube", "Entity", "setModel_(_)", 0), &wrenEntitySetModel},
      {makeKey("kube", "Entity", "setPosition_(_,_,_)", 0), &wrenEntitySetPosition},
      {makeKey("kube", "Entity", "setShader_(_)", 0), &wrenEntitySetShader},
      {makeKey("kube", "Entity", "setSpin_(_,_,_)", 0), &wrenEntitySetSpin},
      {makeKey("kube", "Window", "open(_,_,_)", 1), &wrenOpenWindow}};
  return table;
}

WrenForeignClassMethods wrenBindForeignClass(WrenVM *vm, const char *module,
                                             const char *className) {
  KUBE_DEBUG << "foreign_class:" << module << "." << className;

  WrenForeignClassMethods methods{};

  if (kube::streq(module, "kube")) {
    if (kube::streq(className, "Entity")) {
      methods.allocate = wrenEntityAlloc;
      methods.finalize = wrenEntityDealloc;
    } else if (kube::streq(className, "Model")) {
      methods.allocate = wrenModelAlloc;
      methods.finalize = wrenModelDealloc;
    } else if (kube::streq(className, "Camera")) {
      methods.allocate = wrenCameraAlloc;
      methods.finalize = wrenCameraDealloc;
    } else if (kube::streq(className, "Shader")) {
      methods.allocate = wrenShaderAlloc;
      methods.finalize = wrenShaderDealloc;
    }
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

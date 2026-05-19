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

#include <glm/glm.hpp>
#include <kube/logging.h>
#include <kube/graphics.h>
#include <map>
#include <stdexcept>
#include <typeindex>

namespace kube {

#define EntityID int

// ============================================================================
// Forward declarations
// ============================================================================

class IComponentStore {
public:
  IComponentStore() = default;
  virtual ~IComponentStore() = default;

  virtual bool Contains(EntityID entityID) = 0;
};

template <typename C> class ComponentStore;

typedef struct ModelComponent {
  kube::model model;
} ModelComponent;

typedef struct PositionComponent {
  glm::vec3 position = glm::vec3(0.f);
  glm::mat4 rotation = glm::mat4(1.f);
} PositionComponent;

typedef struct MovementComponent {
  glm::vec3 spin;
  glm::vec3 velocity;
} MovementComponent;

typedef struct GraphicsComponent {
  std::shared_ptr<kube::shader> shader;
} GraphicsComponent;

// ============================================================================
// Entity
// ============================================================================

typedef struct Entity {
  EntityID id;
} Entity;

class EntityStore {
public:
  EntityStore();
  ~EntityStore();

  std::vector<Entity> *All();
  EntityID Create();
  void AddEntity(Entity entity);
  std::vector<Entity> GetEntitiesWithModelComponent();

  std::vector<Entity> GetEntitiesWithComponent(std::type_index t);
  bool EntityHasComponent(EntityID entityID, std::type_index componentType);

  template <typename C> void SetComponent(EntityID entityID, C &&component);

  template <typename C> C *GetComponent(EntityID id);

  auto begin() const { return entities_.begin(); }
  auto end() const { return entities_.end(); }

private:
  template <typename C> ComponentStore<C> *GetStore();

  std::vector<Entity> entities_;

  // Component Stores
  std::map<std::type_index, std::unique_ptr<IComponentStore>> stores_;
};

// ============================================================================
// Components
// ============================================================================

// Publicly inherit IComponentStore for storage in EntityStore.stores_;
template <typename C> class ComponentStore : public IComponentStore {
public:
  void Set(C &&component, EntityID entityID);
  C *Get(EntityID entityID);
  bool Contains(EntityID entityID);

private:
  std::map<EntityID, C> storage_;
};

// ============================================================================
// ComponentStore impl
// ============================================================================

template <typename C> void ComponentStore<C>::Set(C &&component, EntityID entityID) {
  storage_[entityID] = std::move(component);
}

template <typename C> C *ComponentStore<C>::Get(EntityID entityID) {
  auto entry = storage_.find(entityID);
  if (entry == storage_.end())
    return nullptr;
  return &entry->second;
}

template <typename C> bool ComponentStore<C>::Contains(EntityID entityID) {
  return storage_.find(entityID) != storage_.end();
}

// ============================================================================
// EntityStore impl
// ============================================================================

EntityStore::EntityStore() {
  stores_.emplace(typeid(ModelComponent),
                  std::unique_ptr<IComponentStore>(new ComponentStore<ModelComponent>()));
  stores_.emplace(typeid(PositionComponent),
                  std::unique_ptr<IComponentStore>(new ComponentStore<PositionComponent>()));
  stores_.emplace(typeid(MovementComponent),
                  std::unique_ptr<IComponentStore>(new ComponentStore<MovementComponent>()));
  stores_.emplace(typeid(GraphicsComponent),
                  std::unique_ptr<IComponentStore>(new ComponentStore<GraphicsComponent>()));
}

EntityStore::~EntityStore() {}

void EntityStore::AddEntity(Entity entity) { entities_.push_back(entity); }

EntityID EntityStore::Create() {
  Entity entity = Entity{.id = (EntityID)entities_.size()};
  entities_.push_back(entity);
  return entity.id;
}

template <typename C> ComponentStore<C> *EntityStore::GetStore() {
  auto it = stores_.find(typeid(C));
  if (it == stores_.end())
    return nullptr;
  return dynamic_cast<ComponentStore<C> *>(it->second.get());
}

template <typename C> void EntityStore::SetComponent(EntityID entityID, C &&component) {
  auto store = GetStore<C>();
  if (store == nullptr) {
    throw std::invalid_argument("component type not found");
  }
  store->Set(std::move(component), entityID);
}

template <typename C> C *EntityStore::GetComponent(EntityID entityID) {
  auto store = GetStore<C>();
  if (store == nullptr) {
    throw std::invalid_argument("component type not found");
  }
  return store->Get(entityID);
}

std::vector<Entity> EntityStore::GetEntitiesWithComponent(std::type_index componentType) {
  std::vector<Entity> entities;
  for (auto entity : entities_) {
    if (EntityHasComponent(entity.id, componentType)) {
      entities.push_back(entity);
    }
  }
  return entities;
}

bool EntityStore::EntityHasComponent(EntityID entityID, std::type_index componentType) {
  auto store = stores_.find(componentType);
  if (store == stores_.end())
    return false;
  return store->second->Contains(entityID);
}

} // namespace kube

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
#include <kube/model.h>
#include <map>
#include <stdexcept>

namespace kube {

// ----------------------------------------------------------------------------
// Forward declarations
// ----------------------------------------------------------------------------
template <typename C> class ComponentStore;

typedef struct ModelComponent {
  kube::Model model;
} ModelComponent;

typedef struct PositionComponent {
  glm::vec3 position;
} PositionComponent;

typedef struct MovementComponent {
  glm::vec3 spin;
  glm::vec3 velocity;
} MovementComponent;

// ----------------------------------------------------------------------------
// Entity
// ----------------------------------------------------------------------------

#define MAX_ENTITIES 50000
#define EntityID int

typedef struct Entity {
  EntityID id;
} Entity;

class EntityStore {
public:
  EntityStore();
  ~EntityStore();

  EntityID CreateEntity();
  void AddEntity(Entity entity);
  std::vector<Entity> GetEntitiesWithModelComponent();

  // Component modifiers
  void AddModelComponent(EntityID entityID, ModelComponent &&component);
  ModelComponent *GetModelComponent(EntityID entityID);
  void AddPositionComponent(EntityID entityID, PositionComponent &&component);
  PositionComponent *GetPositionComponent(EntityID entityID);
  void AddMovementComponent(EntityID entityID, MovementComponent &&component);
  MovementComponent *GetMovementComponent(EntityID entityID);

private:
  std::vector<Entity> entities_;

  // Component Stores
  ComponentStore<ModelComponent> *models_;
  ComponentStore<PositionComponent> *positions_;
  ComponentStore<MovementComponent> *movements_;
};

// ----------------------------------------------------------------------------
// Components
// ----------------------------------------------------------------------------

template <typename C> class ComponentStore {
public:
  void Set(C &&component, EntityID entityID);
  C *Get(EntityID entityID);

private:
  C storage_[MAX_ENTITIES];
};

template <typename C> void ComponentStore<C>::Set(C &&component, EntityID entityID) {
  if (entityID >= MAX_ENTITIES) {
    KUBE_ERROR << "maximum number of components reached";
    return;
  }

  storage_[entityID] = std::move(component);
}

template <typename C> C *ComponentStore<C>::Get(EntityID entityID) {
  if (entityID >= MAX_ENTITIES) {
    throw std::out_of_range("entity ID out of bounds");
  }

  return &storage_[entityID];
}

EntityStore::EntityStore() {
  models_ = new ComponentStore<ModelComponent>();
  positions_ = new ComponentStore<PositionComponent>();
  movements_ = new ComponentStore<MovementComponent>();
}

EntityStore::~EntityStore() {
  delete models_;
  models_ = nullptr;

  delete positions_;
  positions_ = nullptr;
}

void EntityStore::AddEntity(Entity entity) { entities_.push_back(entity); }

EntityID EntityStore::CreateEntity() {
  Entity entity = Entity{.id = (EntityID)entities_.size()};
  entities_.push_back(entity);
  return entity.id;
}

void EntityStore::AddModelComponent(EntityID entityID, ModelComponent &&component) {
  models_->Set(std::move(component), entityID);
}

ModelComponent *EntityStore::GetModelComponent(EntityID entityID) { return models_->Get(entityID); }

void EntityStore::AddPositionComponent(EntityID entityID, PositionComponent &&component) {
  positions_->Set(std::move(component), entityID);
}

PositionComponent *EntityStore::GetPositionComponent(EntityID entityID) {
  return positions_->Get(entityID);
}

void EntityStore::AddMovementComponent(EntityID entityID, MovementComponent &&component) {
  movements_->Set(std::move(component), entityID);
}

MovementComponent *EntityStore::GetMovementComponent(EntityID entityID) {
  return movements_->Get(entityID);
}

std::vector<Entity> EntityStore::GetEntitiesWithModelComponent() {
  // TODO: Filter
  std::vector<Entity> entities;
  for (auto entity : entities_) {
    entities.push_back(entity);
  }
  return entities;
}

} // namespace kube

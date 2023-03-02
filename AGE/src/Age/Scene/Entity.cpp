//
// Created by alex on 2/16/23.
//

#include "agepch.h"

#include "Entity.h"

namespace AGE {
  const Entity Entity::Null{entt::null, nullptr};

  Entity::Entity(entt::entity entity, Scene* scene) : m_EntityHandle(entity), m_Scene(scene) {
  }
}

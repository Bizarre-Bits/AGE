//
// Created by alex on 2/16/23.
//

#include "Entity.h"

namespace AGE {
  Entity::Entity(entt::entity entity, Scene* scene) : m_EntityHandle(entity), m_Scene(scene) {
  }
}

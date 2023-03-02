//
// Created by alex on 2/16/23.
//

#pragma once

#include "Scene.h"
#include <entt/entt.hpp>

namespace AGE {
  class Entity {
  public:
    static const Entity Null;

    Entity() = default;
    Entity(entt::entity, Scene* scene);
    Entity(const Entity& other) = default;

    template<typename T, typename... Args>
    T& AddComponent(Args&&... args) {
      AGE_CORE_ASSERT(!HasComponent<T>(), "Entity already has the component");

      return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
    }

    template<class T>
    bool HasComponent() const {
      return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
    }

    template<typename T>
    T& GetComponent() {
      AGE_CORE_ASSERT(HasComponent<T>(), "Entity does not have the component");

      return m_Scene->m_Registry.get<T>(m_EntityHandle);
    }

    template<typename T>
    void RemoveComponent() {
      AGE_CORE_ASSERT(HasComponent<T>(), "Entity does not have the component");

      m_Scene->m_Registry.remove<T>(m_EntityHandle);
    }

    operator bool() const { return m_EntityHandle != entt::null; }
    operator uint32_t() const { return (uint32_t)m_EntityHandle; }
    operator entt::entity() const { return m_EntityHandle; }

    bool operator==(const Entity& other) const {
      return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
    }

    bool operator!=(const Entity& other) const {
      return !(*this == other);
    }

  private:
    entt::entity m_EntityHandle{entt::null};

    // TODO: Make weak ptr. Bother with it not sooner than self-made smart-pointer system
    Scene* m_Scene{nullptr};

    friend class Scene;
  };
}// namespace AGE
//
// Created by alex on 2/15/23.
//

#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "Age/Core/Timer.h"

namespace AGE {
  class Entity;

  class Scene {
  public:
    Scene();
    ~Scene() = default;

    Entity CreateEntity(const age_string_t& name = "");
    void DestroyEntity(Entity entity);

    void OnUpdate(Timestep ts);
    void OnViewportResize(uint32_t width, uint32_t height);



  private:
    entt::registry m_Registry;

    friend class Entity;
    friend class SceneOutlinePanel;
    friend class SceneSerializer;
  };

}// namespace AGE

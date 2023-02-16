//
// Created by alex on 2/15/23.
//

#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "Age/Core/Timer.h"
#include "Components.h"

namespace AGE {

  class Scene {
  public:
    Scene();
    ~Scene() = default;

    entt::entity CreateEntity();

    //TODO: Temporary solution. MUST be replaced as we move on to our own entt wrapper
    inline entt::registry& Reg() { return m_Registry; }

    void OnUpdate(Timestep ts);

  private:
    entt::registry m_Registry;
  };

}// namespace AGE

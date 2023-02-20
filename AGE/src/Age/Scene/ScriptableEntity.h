//
// Created by alex on 2/20/23.
//

#pragma once

#include "Entity.h"

namespace AGE {

  class ScriptableEntity {
  public:
    virtual ~ScriptableEntity() = default;

    template<typename T>
    inline T& GetComponent() { return m_Entity.GetComponent<T>(); }

    virtual void OnCreate() {}
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnDestroy() {}

  private:
    Entity m_Entity;
    friend class Scene;
  };

}// namespace AGE

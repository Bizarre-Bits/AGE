//
// Created by alex on 29.08.22.
//

#pragma once

#include "Age/Core/Core.h"
#include "Age/Events/Event.h"
#include "Timer.h"

namespace AGE {
  class Layer {
  public:
    explicit Layer(const age_string_t& debugName = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnEvent(Event& e) {}
    virtual void OnUiRender(Timestep ts) {}

    inline const age_string_t& Name() const { return m_DebugName; }

  protected:
    age_string_t m_DebugName;
  };

}// namespace AGE

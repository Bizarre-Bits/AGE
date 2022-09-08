//
// Created by alex on 29.08.22.
//

#ifndef AGE_LAYER_H
#define AGE_LAYER_H

#include "Age/Core/Core.h"
#include "Age/Events/Event.h"

namespace AGE {
  class  Layer {
  public:
    explicit Layer(const age_string_t& debugName = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}
    virtual void OnEvent(Event& e) {}

    inline const age_string_t& Name() const { return m_DebugName; }

  protected:
    age_string_t m_DebugName;
  };

}// namespace AGE

#endif//AGE_LAYER_H

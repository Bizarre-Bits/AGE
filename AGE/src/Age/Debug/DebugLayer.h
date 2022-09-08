//
// Created by alex on 08.09.22.
//

#ifndef AGE_DEBUGLAYER_H
#define AGE_DEBUGLAYER_H

#include "Age/ImGui/ImGuiLayer.h"

namespace AGE {
  class DebugLayer : public ImGuiLayer {
  public:
    virtual void OnUpdate() override;
  };

}// namespace AGE

#endif//AGE_DEBUGLAYER_H

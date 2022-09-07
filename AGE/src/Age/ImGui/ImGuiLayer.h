//
// Created by alex on 05.09.22.
//

#ifndef AGE_IMGUILAYER_H
#define AGE_IMGUILAYER_H

#include "Age/Core/Core.h"
#include "Age/Core/Layer.h"

#include "Age/ImGui/ImGuiBuild.h"

#include "Age/Events/KeyEvent.h"
#include "Age/Events/MouseEvent.h"
#include "Age/Events/WindowEvent.h"

#include "Age/Events/MouseEvent.h"

namespace AGE {

  class  ImGuiLayer : public Layer {
  public:
    ImGuiLayer();
    ~ImGuiLayer() override;

    void OnAttach() override;
    void OnDetach() override;

    virtual void Begin();
    virtual void End();

  private:
    float m_Time;
  };

}// namespace AGE

#endif//AGE_IMGUILAYER_H

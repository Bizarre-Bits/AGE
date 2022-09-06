//
// Created by alex on 05.09.22.
//

#ifndef AGE_IMGUILAYER_H
#define AGE_IMGUILAYER_H

#include "Age/Core/Core.h"
#include "Age/Core/Layer.h"

#include "Age/Events/KeyEvent.h"
#include "Age/Events/MouseEvent.h"
#include "Age/Events/WindowEvent.h"

#include "Age/Events/MouseEvent.h"

namespace AGE {

  class DLL_PUBLIC ImGuiLayer : public Layer {
  public:
    ImGuiLayer();
    ~ImGuiLayer() override;

    void OnUpdate() override;
    void OnEvent(Event& e) override;
    void OnAttach() override;
    void OnDetach() override;

  private:
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
    bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
    bool OnMouseMovedEvent(MouseMovedEvent& e);
    bool OnMouseScrolledEvent(MouseScrolledEvent& e);
    bool OnWindowResizeEvent(WindowResizeEvent& e);
    bool OnKeyTypedEvent(KeyTypedEvent& e);
    bool OnKeyPressedEvent(KeyPressedEvent& e);
    bool OnKeyReleasedEvent(KeyReleasedEvent& e);

  private:
    float m_Time;
  };

}// namespace AGE

#endif//AGE_IMGUILAYER_H

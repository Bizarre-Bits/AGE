//
// Created by alex on 2/5/23.
//

#ifndef AGE_EDITORLAYER_H
#define AGE_EDITORLAYER_H

#include <Age/Age.h>

namespace AGE {

  class EditorLayer : public Layer {
  public:
    EditorLayer();
    virtual void OnUpdate(Timestep ts) override;
    virtual void OnUiRender(Timestep ts) override;
    virtual void OnEvent(Event& e) override;
    virtual void OnAttach() override;

  private:
    glm::vec2 m_ViewportSize;
    Ref<Framebuffer> m_Framebuffer;
    Ref<Scene> m_ActiveScene;
    OrthographicCameraController m_ViewportCameraController;

    bool m_IsViewportHovered{false};
    bool m_IsViewportFocused{false};
    entt::entity m_Square;
  };

}// namespace AGE

#endif//AGE_EDITORLAYER_H

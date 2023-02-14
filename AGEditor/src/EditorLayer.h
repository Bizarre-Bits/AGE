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

  private:
    glm::vec2                    m_ViewportSize;
    Ref <Framebuffer>            m_Framebuffer;
    OrthographicCameraController m_ViewportCameraController;
  };

} // AGE

#endif //AGE_EDITORLAYER_H

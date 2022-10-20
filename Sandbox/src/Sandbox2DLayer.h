//
// Created by alex on 20.10.22.
//

#ifndef AGE_SANDBOX2DLAYER_H
#define AGE_SANDBOX2DLAYER_H

#include "Age/Age.h"

class Sandbox2DLayer : public AGE::Layer {
public:
  Sandbox2DLayer();
  virtual void OnAttach() override;
  virtual void OnDetach() override;

  virtual void OnUpdate(AGE::Timestep ts) override;
  virtual void OnEvent(AGE::Event& e) override;
private:
  void RenderImGui();

private:
  AGE::OrthographicCameraController m_CameraController;
  AGE::Ref<AGE::Shader>             m_Shader;
  AGE::Ref<AGE::Texture2D>   m_Texture;
  glm::vec3                  m_BgColor{0.2f, 0.2f, 0.2f};
  AGE::Ref<AGE::VertexArray> m_VAO;
};


#endif //AGE_SANDBOX2DLAYER_H

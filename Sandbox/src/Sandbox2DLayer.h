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
  AGE::OrthographicCameraController m_CameraController;
  AGE::Ref<AGE::Texture2D>          m_Texture;
};

class Sandbox2DUI : public AGE::ImGuiLayer {
public:
  virtual void OnUpdate(AGE::Timestep ts) override;
  virtual void OnAttach() override;

private:
  glm::vec3 m_BgColor{0.2f, 0.2f, 0.2f};
};


#endif //AGE_SANDBOX2DLAYER_H

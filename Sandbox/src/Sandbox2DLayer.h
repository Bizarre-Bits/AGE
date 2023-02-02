//
// Created by alex on 20.10.22.
//

#ifndef AGE_SANDBOX2DLAYER_H
#define AGE_SANDBOX2DLAYER_H

#include "Age/Age.h"
#include "Particle.h"

class Sandbox2DUI;

class Sandbox2DLayer : public AGE::Layer {
public:
  Sandbox2DLayer();

  virtual void OnUpdate(AGE::Timestep ts) override;
  virtual void OnEvent(AGE::Event& e) override;
  virtual void OnUiRender(AGE::Timestep ts) override;

  void AppDockSpace(bool* p_open);

private:
  AGE::OrthographicCameraController m_CameraController;
  AGE::Ref<AGE::Framebuffer>        m_Framebuffer;
};


#endif //AGE_SANDBOX2DLAYER_H

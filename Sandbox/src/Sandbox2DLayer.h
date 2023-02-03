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

private:
  AGE::OrthographicCameraController m_CameraController;
};


#endif //AGE_SANDBOX2DLAYER_H

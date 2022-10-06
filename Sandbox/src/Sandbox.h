//
// Created by alex on 04.10.22.
//

#ifndef AGE_SANDBOX_H
#define AGE_SANDBOX_H

#include "Age/Age.h"

class Sandbox : public AGE::Application {
public:
  Sandbox();
  ~Sandbox();
};

class SandboxLayer : public AGE::Layer {
public:
  SandboxLayer();
  virtual ~SandboxLayer() override;

  virtual void OnUpdate(AGE::Timestep ts) override;

private:
  AGE::VertexArray* m_TriangleVA;
  AGE::VertexArray* m_SquareVA;
  AGE::Shader     * m_Shader;
  AGE::OrthographicCamera m_Camera;

  float m_CameraSpeed;
};


#endif //AGE_SANDBOX_H

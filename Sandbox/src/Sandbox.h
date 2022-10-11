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
  AGE::Shader     * m_TriangleShader;
  AGE::Shader     * m_SquareShader;
  AGE::OrthographicCamera m_Camera;
  glm::vec4               m_SquareColor;

  glm::vec3 m_TrianglePos;
  float     m_CameraSpeed;
  float     m_TriangleSpeed;
  float     m_TriangleRotation;
};


#endif //AGE_SANDBOX_H

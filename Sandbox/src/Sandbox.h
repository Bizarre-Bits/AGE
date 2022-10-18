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
  virtual void OnEvent(AGE::Event& e) override;

private:
  AGE::ShaderLibrary                m_ShaderLibrary;
  AGE::Ref<AGE::VertexArray>        m_SquareVA;
  AGE::Ref<AGE::Texture2D>          m_CheckerBoardTex, m_LetterATex;
  AGE::OrthographicCameraController m_CameraController;
  AGE::OrthographicCamera           m_Camera;
};


#endif//AGE_SANDBOX_H

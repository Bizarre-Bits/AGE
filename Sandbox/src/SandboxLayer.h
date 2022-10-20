//
// Created by alex on 20.10.22.
//

#ifndef AGE_SANDBOXLAYER_H
#define AGE_SANDBOXLAYER_H

#include "glm/gtc/type_ptr.hpp"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Age/Age.h"

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

#endif //AGE_SANDBOXLAYER_H

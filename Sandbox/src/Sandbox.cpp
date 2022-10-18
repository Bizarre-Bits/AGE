//
// Created by alex on 04.10.22.
//

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Sandbox.h"
#include "glm/gtc/type_ptr.hpp"

AGE::Application* AGE::CreateApplication() {
  return new Sandbox;
}

Sandbox::Sandbox() : AGE::Application() {
  PushLayer(new SandboxLayer);
}

Sandbox::~Sandbox() {}

// Sandbox Layer

SandboxLayer::SandboxLayer() : m_Camera(-1.6, 1.6, -0.9, 0.9) {
  m_SquareVA = AGE::Ref<AGE::VertexArray>(AGE::VertexArray::Create());

  float vertices[4 * 5]{
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.0f, 1.0f, 1.0f
  };

  AGE::Ref<AGE::VertexBuffer> vb = AGE::Ref<AGE::VertexBuffer>(
      AGE::VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float)));

  AGE::BufferLayout layout{
      {"a_Pos",      AGE::ShaderDataType::Float3},
      {"a_TexCoord", AGE::ShaderDataType::Float2}
  };

  vb->SetLayout(layout);

  uint32_t indices[6]{0, 1, 2, 0, 2, 3};

  AGE::Ref<AGE::IndexBuffer> ib = AGE::Ref<AGE::IndexBuffer>(
      AGE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

  m_SquareVA->AddVertexBuffer(vb);
  m_SquareVA->SetIndexBuffer(ib);

  //Shaders

  auto flatColorShader = m_ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
  m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

  m_CheckerBoardTex = AGE::Texture2D::Create("assets/textures/Checkerboard.png");
  m_LetterATex      = AGE::Texture2D::Create("assets/textures/letter_a.png");

  std::dynamic_pointer_cast<AGE::OpenGLShader>(flatColorShader)->UploadUniformInt(
      "u_Texture", 0
  );
}

SandboxLayer::~SandboxLayer() {

}

void SandboxLayer::OnUpdate(AGE::Timestep ts) {
  AGE::Renderer::BeginScene(m_Camera);

  AGE::RenderCommand::Clear();


  glm::vec3 redColor(0.6f, 0.2f, 0.3f);
  glm::vec3 blueColor(0.3f, 0.2f, 0.6f);

  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));

  glm::mat4 redTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, -0.25f, 0.0f))
                           * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

  auto flatColor = m_ShaderLibrary.Get("FlatColor");

  flatColor->Bind();
  std::dynamic_pointer_cast<AGE::OpenGLShader>(flatColor)->UploadUniformFloat3(
      "u_Color", blueColor
  );
  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 20; x++) {
      glm::vec3 pos(x * 0.06f, y * 0.06f, 0.0f);
      glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
      AGE::Renderer::Submit(m_SquareVA, flatColor, transform);
    }
  }

  auto textureShader = m_ShaderLibrary.Get("Texture");

  m_CheckerBoardTex->Bind();
  AGE::Renderer::Submit(m_SquareVA, textureShader, redTransform);

  m_LetterATex->Bind();
  AGE::Renderer::Submit(m_SquareVA, textureShader, redTransform);

  AGE::Renderer::EndScene();
}

//
// Created by alex on 20.10.22.
//

#include "Sandbox2DLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include "glm/gtc/type_ptr.hpp"

Sandbox2DLayer::Sandbox2DLayer() : m_CameraController(1280.0f / 720.0f) {

}


void Sandbox2DLayer::OnAttach() {
  AGE::RenderCommand::SetClearColor({m_BgColor, 1.0f});

  m_Texture = AGE::Texture2D::Create("assets/textures/creeper-face.png");
  m_Shader  = AGE::Shader::Create("assets/shaders/Texture.glsl");

  m_Shader->Bind();
  m_Texture->Bind();
  std::static_pointer_cast<AGE::OpenGLShader>(m_Shader)->UploadUniformInt("u_Texture", 0);

  m_VAO = AGE::Ref<AGE::VertexArray>(AGE::VertexArray::Create());

  float vertices[4 * 5]{
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
  };

  auto VBO = AGE::Ref<AGE::VertexBuffer>(
      AGE::VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float))
  );

  AGE::BufferLayout layout{
      {"a_Pos", AGE::ShaderDataType::Float3},
      {"a_Tex", AGE::ShaderDataType::Float2}
  };

  VBO->SetLayout(layout);

  m_VAO->AddVertexBuffer(VBO);

  uint32_t indices[6]{0, 1, 2, 0, 2, 3};

  auto IB = AGE::Ref<AGE::IndexBuffer>(
      AGE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t))
  );

  m_VAO->SetIndexBuffer(IB);
}

void Sandbox2DLayer::OnDetach() {
  Layer::OnDetach();
}

void Sandbox2DLayer::OnUpdate(AGE::Timestep ts) {
  RenderImGui();

  m_CameraController.OnUpdate(ts);

  AGE::Renderer::BeginScene(m_CameraController.GetCamera());

  AGE::RenderCommand::Clear();

  AGE::Renderer::Submit(m_VAO, m_Shader);

  AGE::Renderer::EndScene();
}

void Sandbox2DLayer::OnEvent(AGE::Event& e) {
  m_CameraController.OnEvent(e);
}

void Sandbox2DLayer::RenderImGui() {
  ImGui::Begin("Background");
  if (ImGui::ColorEdit3("Color", glm::value_ptr(m_BgColor))) {
    AGE::RenderCommand::SetClearColor(glm::vec4(m_BgColor, 1.0f));
  }
  ImGui::End();
}

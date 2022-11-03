//
// Created by alex on 20.10.22.
//

#include "Sandbox2DLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Age/Age.h"

#include "glm/gtc/type_ptr.hpp"

Sandbox2DLayer::Sandbox2DLayer() : m_CameraController(1280.0f / 720.0f) {
  m_Texture = AGE::Texture2D::Create("assets/textures/creeper-face.png");
}


void Sandbox2DLayer::OnAttach() {
}

void Sandbox2DLayer::OnDetach() {
  Layer::OnDetach();
}

void Sandbox2DLayer::OnUpdate(AGE::Timestep ts) {

  m_CameraController.OnUpdate(ts);

  AGE::RenderCommand::Clear();
  AGE::Renderer2D::BeginScene(m_CameraController.GetCamera());

  AGE::Renderer2D::DrawQuad(
      glm::vec2{-0.5f, 0.0f}, glm::vec2{0.4f}, m_Texture
  );

  AGE::Renderer2D::DrawQuad(
      glm::vec2{0.5f, 0.0f}, glm::vec2{0.4f}, m_Texture, {1.0f, 1.0f, 5.0f, 1.0f}
  );

  AGE::Renderer2D::EndScene();
}

void Sandbox2DLayer::OnEvent(AGE::Event& e) {
  m_CameraController.OnEvent(e);
}

void Sandbox2DUI::OnAttach() {
  ImGuiLayer::OnAttach();

  AGE::RenderCommand::SetClearColor({m_BgColor, 1.0f});
}

void Sandbox2DUI::OnUpdate(AGE::Timestep ts) {
  ImGui::Begin("Background");
  if (ImGui::ColorEdit3("Color", glm::value_ptr(m_BgColor))) {
    AGE::RenderCommand::SetClearColor(glm::vec4(m_BgColor, 1.0f));
  }
  ImGui::End();
}

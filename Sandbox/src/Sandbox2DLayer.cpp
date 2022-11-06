//
// Created by alex on 20.10.22.
//

#include "Sandbox2DLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Age/Age.h"

#include "glm/gtc/type_ptr.hpp"

Sandbox2DLayer::Sandbox2DLayer() : m_CameraController(1280.0f / 720.0f) {
  m_CreeperFaceTex  = AGE::Texture2D::Create("assets/textures/creeper-face.png");
  m_IncorrectTex    = AGE::Texture2D::Create("wrong/path/to.png");
  m_CheckerboardTex = AGE::Texture2D::Create("assets/textures/Checkerboard.png");
}


void Sandbox2DLayer::OnAttach() {
}

void Sandbox2DLayer::OnDetach() {
  Layer::OnDetach();
}

void Sandbox2DLayer::OnUpdate(AGE::Timestep ts) {
  AGE_PROFILE_FUNCTION();

  m_CameraController.OnUpdate(ts);

  AGE::RenderCommand::Clear();
  AGE::Renderer2D::BeginScene(m_CameraController.GetCamera());

  {
    AGE_PROFILE_SCOPE("Sandbox2D Render");
    AGE::Renderer2D::DrawQuad(
        glm::vec2{0.6f, 0.0f}, glm::vec2{0.4f}, m_CreeperFaceTex, {1.0f, 1.0f, 5.0f, 1.0f}
    );

    AGE::Renderer2D::DrawQuad(
        glm::vec2{-0.6f, 0.0f}, glm::vec2{0.4f}, m_CreeperFaceTex
    );

    AGE::Renderer2D::DrawQuad(
        glm::vec2{0.0f, 0.6f}, glm::vec2{0.4f}, {0.6f, 0.2f, 0.8f, 1.0f}
    );

    AGE::Renderer2D::DrawQuad(
        glm::vec2{0.0f, -0.6f}, glm::vec2{0.4f}, m_IncorrectTex
    );

    AGE::Renderer2D::DrawQuad(glm::vec3{0.0f, 0.0f, -0.1f}, glm::vec2{10.0f}, m_CheckerboardTex);
  }

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

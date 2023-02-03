//
// Created by alex on 20.10.22.
//

#include "agepch.h"

#include "Sandbox2DLayer.h"
#include "Age/Age.h"

#include "glm/gtc/type_ptr.hpp"

Sandbox2DLayer::Sandbox2DLayer() : m_CameraController(1280.0f / 720.0f) {
  AGE_PROFILE_FUNCTION();

  AGE::FramebufferSpecification fbSpec{};

  fbSpec.Width  = AGE::Application::Instance()->Window()->Width();
  fbSpec.Height = AGE::Application::Instance()->Window()->Height();

  m_Framebuffer = AGE::Framebuffer::Create(fbSpec);
}

void Sandbox2DLayer::OnUpdate(AGE::Timestep ts) {
  AGE_PROFILE_FUNCTION();

  m_CameraController.OnUpdate(ts);

  AGE::RenderCommand::Clear();
  AGE::Renderer2D::BeginScene(m_CameraController.GetCamera());

  AGE::Renderer2D::DrawQuad(glm::vec3{0, 0.5f, 0.0f}, glm::vec2{1.0f}, glm::vec4{0.5f, 0.3f, 0.55f, 1.0f});

  AGE::Renderer2D::EndScene();
}

void Sandbox2DLayer::OnEvent(AGE::Event& e) {
  AGE_PROFILE_FUNCTION();

  m_CameraController.OnEvent(e);
}

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

  m_Framebuffer->Bind();
  m_CameraController.OnUpdate(ts);

  AGE::RenderCommand::Clear();
  AGE::Renderer2D::BeginScene(m_CameraController.GetCamera());

  AGE::Renderer2D::DrawQuad(glm::vec3{0, 0.5f, 0.0f}, glm::vec2{1.0f}, glm::vec4{0.5f, 0.3f, 0.55f, 1.0f});

  AGE::Renderer2D::EndScene();
  m_Framebuffer->Unbind();
}

void Sandbox2DLayer::AppDockSpace(bool* p_open) {
  // If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
  // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
  // In this specific demo, we are not using DockSpaceOverViewport() because:
  // - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
  // - we allow the host window to have padding (when opt_padding == true)
  // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
  // TL;DR; this demo is more complicated than what you would normally use.
  // If we removed all the options we are showcasing, this demo would become:
  //     void ShowExampleAppDockSpace()
  //     {
  //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  //     }

  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
  // because it would be confusing to have two docking targets within each others.
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGui::SetNextWindowViewport(viewport->ID);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
  // and handle the pass-thru hole, so we ask Begin() to not render a background.
  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
  // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
  // all active windows docked into it will lose their parent and become undocked.
  // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
  // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("DockSpace Demo", p_open, window_flags);


  // Submit the DockSpace
  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }

  ImGui::PopStyleVar(3);

  ImGui::End();
}

void Sandbox2DLayer::OnEvent(AGE::Event& e) {
  AGE_PROFILE_FUNCTION();

  m_CameraController.OnEvent(e);
}

void Sandbox2DLayer::OnUiRender(AGE::Timestep ts) {
  static bool showDemo;
  ImGui::ShowDemoWindow(&showDemo);

  static bool showDockSpace;
  AppDockSpace(&showDockSpace);

  ImGui::Begin("Settings");
  uint32_t textureID = m_Framebuffer->ColorAttachmentID();
  ImGui::Image((void*)textureID, ImVec2{1280.0f / 2, 720.0f / 2}, {0, 1}, {1, 0});
  ImGui::End();
}

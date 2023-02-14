//
// Created by alex on 2/5/23.
//

#include "EditorLayer.h"

#include <Age/Renderer/RenderCommand.h>
#include <imgui.h>

namespace AGE {
  EditorLayer::EditorLayer() : m_ViewportCameraController(1.0f) {
    FramebufferSpecification specs;
    specs.Width  = 1;
    specs.Height = 1;

    m_Framebuffer = Framebuffer::Create(specs);
  }

  void EditorLayer::OnUpdate(Timestep ts) {
    m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
    m_ViewportCameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);

    m_Framebuffer->Bind();

    RenderCommand::Clear();
    Renderer2D::BeginScene(m_ViewportCameraController.GetCamera());

    Renderer2D::DrawQuad(glm::vec2{0.0f}, glm::vec2{1.0f}, glm::vec4{1.0f, 0.0f, 1.0f, 1.0f});

    m_ViewportCameraController.OnUpdate(ts);
    Renderer2D::EndScene();

    m_Framebuffer->Unbind();
  }

  void EditorLayer::OnUiRender(Timestep ts) {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Viewport");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

    uint32_t textureID = m_Framebuffer->ColorAttachmentID();
    ImGui::Image((void*)textureID, viewportPanelSize, {0.0f, 1.0f}, {1.0f, 0.0f});

    ImGui::End();

    ImGui::PopStyleVar();

    ImGui::Begin("Viewport Debug");

    ImGui::Text("Viewport: %d %d", (int)m_ViewportSize.x, (int)m_ViewportSize.y);
    ImGui::Text("Framebuffer: %d %d", (int)m_Framebuffer->Specification().Width, (int)m_Framebuffer->Specification().Height);
    ImGui::Text("Aspect ratio: %f", m_ViewportSize.x / m_ViewportSize.y);
    ImGui::Text("Camera aspect ratio: %f", m_ViewportCameraController.AspectRatio());

    ImGui::End();
  }

  void EditorLayer::OnEvent(Event& e) {
    Layer::OnEvent(e);
    m_ViewportCameraController.OnEvent(e);
  }

} // AGE

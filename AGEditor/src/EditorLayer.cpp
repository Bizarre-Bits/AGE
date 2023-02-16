//
// Created by alex on 2/5/23.
//

#include "EditorLayer.h"

#include <Age/Renderer/RenderCommand.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace AGE {
  EditorLayer::EditorLayer() : m_ViewportCameraController(1.0f), m_ViewportSize(0.0f) {
    FramebufferSpecification specs;
    specs.Width  = 1;
    specs.Height = 1;

    m_Framebuffer = Framebuffer::Create(specs);
  }

  void EditorLayer::OnAttach() {
    Layer::OnAttach();

    m_ActiveScene = MakeRef<Scene>();
    m_Square      = m_ActiveScene->CreateEntity();
    auto& reg     = m_ActiveScene->Reg();
    reg.emplace<SpriteComponent>(m_Square, glm::vec4{1.0f, 0.0f, 1.0f, 1.0f});
    reg.emplace<TransformComponent>(m_Square);
  }

  void EditorLayer::OnUpdate(Timestep ts) {

    m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_ViewportCameraController.OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

    m_Framebuffer->Bind();

    RenderCommand::Clear();
    Renderer2D::BeginScene(m_ViewportCameraController.GetCamera());

    m_ActiveScene->OnUpdate(ts);

    if (m_IsViewportFocused)
      m_ViewportCameraController.OnUpdate(ts);
    else
      m_ViewportCameraController.GetCamera().RecalculateViewProjectionMatrix();

    Renderer2D::EndScene();

    m_Framebuffer->Unbind();
  }

  void EditorLayer::OnUiRender(Timestep ts) {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGuiWindowClass viewportWindowClass{};
    viewportWindowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&viewportWindowClass);
    ImGui::Begin("Viewport");

    m_IsViewportHovered = ImGui::IsWindowHovered();
    m_IsViewportFocused = ImGui::IsWindowFocused();


    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize           = {viewportPanelSize.x, viewportPanelSize.y};

    uint32_t textureID = m_Framebuffer->ColorAttachmentID();
    ImGui::Image((void*)textureID, viewportPanelSize, {0.0f, 1.0f}, {1.0f, 0.0f});

    ImGui::End();

    ImGui::PopStyleVar();

    ImGui::Begin("Viewport Debug");

    ImGui::Text("Viewport: %d %d", (int)m_ViewportSize.x, (int)m_ViewportSize.y);
    ImGui::Text("Framebuffer: %d %d", (int)m_Framebuffer->Specification().Width, (int)m_Framebuffer->Specification().Height);
    ImGui::Text("Aspect ratio: %f", m_ViewportSize.x / m_ViewportSize.y);
    ImGui::Text("Camera aspect ratio: %f", m_ViewportCameraController.AspectRatio());
    ImGui::Text("Focus: %b", m_IsViewportFocused);
    ImGui::Text("Hover: %b", m_IsViewportHovered);

    glm::vec4& tint = m_ActiveScene->Reg().get<SpriteComponent>(m_Square).Tint;
    ImGui::ColorEdit3("Square Tint", glm::value_ptr(tint));
    ImGui::End();
  }

  void EditorLayer::OnEvent(Event& e) {
    Layer::OnEvent(e);
    if (!m_IsViewportHovered) {
      ImGuiIO& io = ImGui::GetIO();
      e.Handled |= e.IsInCategory(EventCategory::EventCategoryMouse) & io.WantCaptureMouse;
      e.Handled |= e.IsInCategory(EventCategory::EventCategoryKeyboard) & io.WantCaptureKeyboard;
    } else {
      m_ViewportCameraController.OnEvent(e);
    }
  }

}// namespace AGE

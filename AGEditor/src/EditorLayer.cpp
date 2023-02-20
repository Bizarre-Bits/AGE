//
// Created by alex on 2/5/23.
//

#include "EditorLayer.h"

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

    RenderCommand::SetClearColor(m_BgColor);

    m_ActiveScene  = MakeRef<Scene>();
    m_SquareEntity = m_ActiveScene->CreateEntity("Square");
    m_SquareEntity.AddComponent<SpriteComponent>(glm::vec4{1.0f, 0.0f, 1.0f, 1.0f});

    m_CameraEntity          = m_ActiveScene->CreateEntity("Primary Camera");
    auto& cameraComponent   = m_CameraEntity.AddComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));
    cameraComponent.Primary = true;

    m_SecondaryCamera                = m_ActiveScene->CreateEntity("Secondary Camera");
    auto& secondaryCameraComponent   = m_SecondaryCamera.AddComponent<CameraComponent>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f));
    secondaryCameraComponent.Primary = true;
  }

  void EditorLayer::OnUpdate(Timestep ts) {

    m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_ViewportCameraController.OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

    m_Framebuffer->Bind();

    RenderCommand::Clear();

    m_ActiveScene->OnUpdate(ts);

    if (m_IsViewportFocused)
      m_ViewportCameraController.OnUpdate(ts);
    else
      m_ViewportCameraController.GetCamera().RecalculateViewProjectionMatrix();

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

    if (ImGui::ColorEdit3("Background", glm::value_ptr(m_BgColor))) {
      RenderCommand::SetClearColor(m_BgColor);
    }

    ImGui::Separator();
    auto& tag  = m_SquareEntity.GetComponent<TagComponent>();
    auto& tint = m_SquareEntity.GetComponent<SpriteComponent>().Tint;

    ImGui::Text("%s", tag.Tag.c_str());
    ImGui::ColorEdit4("Square Tint", glm::value_ptr(tint));

    ImGui::Separator();
    ImGui::Text("Cameras");
    static bool cameraA = true;
    if (ImGui::Checkbox("Camera A", &cameraA)) {
      auto& cameraAComponent   = m_CameraEntity.GetComponent<CameraComponent>();
      auto& cameraBComponent   = m_SecondaryCamera.GetComponent<CameraComponent>();
      cameraAComponent.Primary = cameraA;
      cameraBComponent.Primary = !cameraA;
    }
    auto& cameraPosition = (cameraA ? m_CameraEntity : m_SecondaryCamera).GetComponent<TransformComponent>().Transform[3];
    ImGui::DragFloat2("Camera Transform", glm::value_ptr(cameraPosition));

    ImGui::End();
  }

  void EditorLayer::OnEvent(Event& e) {
    Layer::OnEvent(e);
    if (!m_IsViewportHovered) {
      const ImGuiIO& io = ImGui::GetIO();
      e.Handled |= e.IsInCategory(EventCategory::EventCategoryMouse) & io.WantCaptureMouse;
      e.Handled |= e.IsInCategory(EventCategory::EventCategoryKeyboard) & io.WantCaptureKeyboard;
    } else {
      m_ViewportCameraController.OnEvent(e);
    }
  }

}// namespace AGE

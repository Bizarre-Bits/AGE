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

    m_RedSquareEntity = m_ActiveScene->CreateEntity("Red Square");
    m_RedSquareEntity.AddComponent<SpriteComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

    m_BlueSquareEntity = m_ActiveScene->CreateEntity("Blue Square");
    m_BlueSquareEntity.AddComponent<SpriteComponent>(glm::vec4{0.0f, 0.0f, 1.0f, 1.0f});

    m_CameraA               = m_ActiveScene->CreateEntity("Camera A");
    auto& cameraComponent   = m_CameraA.AddComponent<CameraComponent>();
    cameraComponent.Primary = true;
    cameraComponent.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);

    m_CameraB                        = m_ActiveScene->CreateEntity("Camera B");
    auto& secondaryCameraComponent   = m_CameraB.AddComponent<CameraComponent>();
    secondaryCameraComponent.Primary = false;

    class CameraControllerScript : public ScriptableEntity {
    public:
      virtual ~CameraControllerScript() = default;

      virtual void OnUpdate(Timestep ts) override {
        auto& transform = GetComponent<TransformComponent>().Transform;

        constexpr float speed = 5.0f;

        if(Input::IsKeyPressed(Key::A))
          transform[3][0] -= speed * ts;
        else if(Input::IsKeyPressed(Key::D))
          transform[3][0] += speed * ts;

        if(Input::IsKeyPressed(Key::S))
          transform[3][1] -= speed * ts;
        else if(Input::IsKeyPressed(Key::W))
          transform[3][1] += speed * ts;
      }
    };

    m_CameraA.AddComponent<NativeScriptComponent>().Bind<CameraControllerScript>();

    m_SceneOutlinePanel.SetContext(m_ActiveScene);
  }

  void EditorLayer::OnUpdate(Timestep ts) {

    m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    //    m_ViewportCameraController.OnResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

    m_Framebuffer->Bind();

    RenderCommand::Clear();

    m_ActiveScene->OnUpdate(ts);

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
    auto& tag  = m_RedSquareEntity.GetComponent<TagComponent>();
    auto& tint = m_RedSquareEntity.GetComponent<SpriteComponent>().Tint;

    ImGui::Text("%s", tag.Tag.c_str());
    ImGui::ColorEdit4("Square Tint", glm::value_ptr(tint));

    ImGui::Separator();
    ImGui::Text("Cameras");
    static bool cameraA = true;
    if (ImGui::Checkbox("Camera A", &cameraA)) {
      auto& cameraAComponent   = m_CameraA.GetComponent<CameraComponent>();
      auto& cameraBComponent   = m_CameraB.GetComponent<CameraComponent>();
      cameraAComponent.Primary = cameraA;
      cameraBComponent.Primary = !cameraA;
    }

    ImGui::End();

    m_SceneOutlinePanel.OnUiRender();
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

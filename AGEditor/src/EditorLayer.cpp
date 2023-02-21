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
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.Fonts->AddFontFromFileTTF("assets/client_assets/AGEditor/fonts/Roboto/Roboto-Bold.ttf", 18.0f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/client_assets/AGEditor/fonts/Roboto/Roboto-Regular.ttf", 18.0f);

    ImGui::StyleColorsDark();

    SetDarkThemeColors();

    RenderCommand::SetClearColor(m_BgColor);

    m_ActiveScene = MakeRef<Scene>();

    m_SceneOutlinePanel.SetContext(m_ActiveScene);

    SetupDemoFeatures();
  }

  void EditorLayer::SetupDemoFeatures() {
    m_RedSquareEntity = m_ActiveScene->CreateEntity("Red Square");
    m_RedSquareEntity.AddComponent<SpriteComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});
    auto& redTransform         = m_RedSquareEntity.GetComponent<TransformComponent>();
    redTransform.Translation.x = -0.6f;

    m_BlueSquareEntity = m_ActiveScene->CreateEntity("Blue Square");
    m_BlueSquareEntity.AddComponent<SpriteComponent>(glm::vec4{0.0f, 0.0f, 1.0f, 1.0f});
    auto& blueTransform         = m_BlueSquareEntity.GetComponent<TransformComponent>();
    blueTransform.Translation.x = 0.6f;

    m_CameraA               = m_ActiveScene->CreateEntity("Camera A");
    auto& cameraComponent   = m_CameraA.AddComponent<CameraComponent>();
    cameraComponent.Primary = true;
    cameraComponent.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
    auto& cameraTransform         = m_CameraA.GetComponent<TransformComponent>();
    cameraTransform.Translation.z = 5.0f;

    class CameraControllerScript : public ScriptableEntity {
    public:
      virtual ~CameraControllerScript() = default;

      virtual void OnUpdate(Timestep ts) override {
        auto& transform = GetComponent<TransformComponent>().Translation;

        constexpr float speed = 5.0f;

        if (Input::IsKeyPressed(Key::A))
          transform.x -= speed * ts;
        else if (Input::IsKeyPressed(Key::D))
          transform.x += speed * ts;

        if (Input::IsKeyPressed(Key::S))
          transform.y -= speed * ts;
        else if (Input::IsKeyPressed(Key::W))
          transform.y += speed * ts;
      }
    };

    m_CameraA.AddComponent<NativeScriptComponent>().Bind<CameraControllerScript>();
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
    ImGuiIO& io           = ImGui::GetIO();
    ImGuiStyle& style     = ImGui::GetStyle();
    float windowMinSize   = style.WindowMinSize.x;
    style.WindowMinSize.x = 350.0f;
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    style.WindowMinSize.x = windowMinSize;

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

  void EditorLayer::SetDarkThemeColors() {
    auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

    const ImVec4 interactable{0.2f, 0.205f, 0.21f, 1.0f};
    const ImVec4 interactableHovered{0.3f, 0.305f, 0.31f, 1.0f};
    const ImVec4 interactableActive{0.15f, 0.1505f, 0.151f, 1.0f};

    colors[ImGuiCol_Header]        = interactable;
    colors[ImGuiCol_HeaderHovered] = interactableHovered;
    colors[ImGuiCol_HeaderActive]  = interactableActive;

    colors[ImGuiCol_Button]        = interactable;
    colors[ImGuiCol_ButtonHovered] = interactableHovered;
    colors[ImGuiCol_ButtonActive]  = interactableActive;

    colors[ImGuiCol_FrameBg]        = interactable;
    colors[ImGuiCol_FrameBgHovered] = interactableHovered;
    colors[ImGuiCol_FrameBgActive]  = interactableActive;

    const ImVec4& tabBase{0.15f, 0.1505f, 0.151f, 1.0f};
    const ImVec4& tabHovered{0.38f, 0.3805f, 0.381f, 1.0f};
    const ImVec4& tabActive{0.28f, 0.2805f, 0.281f, 1.0f};

    colors[ImGuiCol_Tab]                = tabBase;
    colors[ImGuiCol_TabHovered]         = tabHovered;
    colors[ImGuiCol_TabActive]          = tabActive;
    colors[ImGuiCol_TabUnfocused]       = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

    colors[ImGuiCol_TitleBg]          = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive]    = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.95f, 0.1505f, 0.951f, 1.0f};
  }

}// namespace AGE

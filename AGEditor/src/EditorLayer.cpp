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
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

    m_Framebuffer->Bind();

    RenderCommand::Clear();

    m_ActiveScene->OnUpdate(ts);

    m_Framebuffer->Unbind();
  }

  void EditorLayer::OnUiRender(Timestep ts) {
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport      = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                   | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("MainDockSpace", nullptr, windowFlags);

    ImGuiID dockID = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockID);

    MainMenuBar();
    Viewport();

    m_SceneOutlinePanel.OnUiRender();

    ImGui::End();

    ImGui::PopStyleVar(3);
  }

  void EditorLayer::Viewport() {
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);

    ImGui::Begin("Viewport");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    this->m_ViewportSize     = {viewportPanelSize.x, viewportPanelSize.y};
    ImGui::Image((void*)(uint64_t)this->m_Framebuffer->ColorAttachmentID(), viewportPanelSize, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});

    ImGui::End();
  }

  void EditorLayer::MainMenuBar() const {
    if (ImGui::BeginMainMenuBar()) {

      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {
          SceneSerializer serializer{m_ActiveScene};
          serializer.Serialize("scene.yaml");
        }
        if (ImGui::MenuItem("Load Scene")) {
          SceneSerializer serializer{m_ActiveScene};
          serializer.Deserialize("scene.yaml");
        }

        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }
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

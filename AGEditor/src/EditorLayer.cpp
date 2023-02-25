//
// Created by alex on 2/5/23.
//

#include "EditorLayer.h"

#include <Age/Utils/FileDialogs.h>
#include <Age/Utils/MathUtils.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace AGE {
  EditorLayer::EditorLayer() : m_ViewportSize(0.0f) {
    FramebufferSpecification specs;
    specs.Width = 1;
    specs.Height = 1;

    m_Framebuffer = Framebuffer::Create(specs);
  }

  void EditorLayer::OnAttach() {
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    io.Fonts->AddFontFromFileTTF(
        "assets/client_assets/AGEditor/fonts/Roboto/Roboto-Bold.ttf", 18.0f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF(
        "assets/client_assets/AGEditor/fonts/Roboto/Roboto-Regular.ttf", 18.0f);

    ImGui::StyleColorsDark();

    SetDarkThemeColors();

    m_ActiveScene = MakeRef<Scene>();

    m_SceneOutlinePanel.SetContext(m_ActiveScene);
  }

  void EditorLayer::OnUpdate(Timestep ts) {
    m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
    m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x,
                                    (uint32_t)m_ViewportSize.y);
    m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

    if (m_ViewportHovered)
      m_EditorCamera.OnUpdate(ts);

    m_Framebuffer->Bind();

    RenderCommand::Clear();

    m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

    m_Framebuffer->Unbind();
  }

  void EditorLayer::OnUiRender(Timestep ts) {
    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoBringToFrontOnFocus |
                   ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("MainDockSpace", nullptr, windowFlags);

    ImGuiID dockID = ImGui::GetID("MainDockSpace");
    ImGui::DockSpace(dockID);

    MainMenuBar();
    Viewport();

    m_SceneOutlinePanel.OnUiRender();

    ImGui::End();

    ImGui::PopStyleVar(2);
  }

  void EditorLayer::Viewport() {
    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");

    m_ViewportHovered = ImGui::IsWindowHovered();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
    this->m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};
    ImGui::Image((void*)(uint64_t)this->m_Framebuffer->ColorAttachmentID(),
                 viewportPanelSize, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});

    Entity selectedEntity = m_SceneOutlinePanel.SelectedEntity();

    if (selectedEntity) {
      ImGuizmo::SetOrthographic(false);
      ImGuizmo::SetDrawlist();

      const ImVec2& windowSize = ImGui::GetWindowSize();
      const ImVec2& windowPos = ImGui::GetWindowPos();

      ImGuizmo::SetRect(windowPos.x, windowPos.y, windowSize.x, windowSize.y);

      auto& tc = selectedEntity.GetComponent<TransformComponent>();
      glm::mat4 entityTransform = tc.Transform();

      bool snap = Input::IsKeyPressed(Key::LeftControl);
      float snapValue = 0.5f;
      if (m_GizmoOperation == ImGuizmo::OPERATION::ROTATE) {
        snapValue = 45.0f;
      }
      float snapValues[3] = {snapValue, snapValue, snapValue};

      ImGuizmo::Manipulate(glm::value_ptr(m_EditorCamera.ViewMatrix()), glm::value_ptr(m_EditorCamera.Projection()),
                           m_GizmoOperation, m_GizmoMode,
                           glm::value_ptr(entityTransform), nullptr,
                           snap ? snapValues : nullptr);

      if (ImGuizmo::IsUsing()) {
        MathUtils::DecomposeTransform(entityTransform, tc.Translation,
                                      tc.Rotation, tc.Scale);
      }
    }

    ImGui::End();
    ImGui::PopStyleVar();
  }

  void EditorLayer::MainMenuBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{5, 5});

    if (ImGui::BeginMainMenuBar()) {

      if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New", "Ctrl+N"))
          CreateNewScene();

        if (ImGui::MenuItem("Open...", "Ctrl+O"))
          OpenSceneDialog();

        if (ImGui::MenuItem("Save", "Ctrl+S")) {
        }

        if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
          SaveSceneAsDialog();

        ImGui::EndMenu();
      }

      ImGui::EndMainMenuBar();
    }

    ImGui::PopStyleVar();
  }

  void EditorLayer::CreateNewScene() {
    m_ActiveScene = MakeRef<Scene>();
    m_SceneOutlinePanel.SetContext(m_ActiveScene);
    m_SceneFilepath = "";
  }

  void EditorLayer::SaveSceneAsDialog() {
    const age_string_t filepath{
        FileDialogs::SaveFile("Scene File (*.agescene)| *.agescene")};

    if (!filepath.empty()) {
      AGE_TRACE("Saving scene to \"{0}\"", filepath);
      m_SceneFilepath = filepath;
      SceneSerializer serializer{m_ActiveScene};
      serializer.Serialize(filepath);
    }
  }

  void EditorLayer::SaveScene() {
    if (m_SceneFilepath.empty()) {
      SaveSceneAsDialog();
      return;
    }

    SceneSerializer serializer{m_ActiveScene};
    serializer.Serialize(m_SceneFilepath);
  }

  void EditorLayer::OpenSceneDialog() {
    const age_string_t filepath{
        FileDialogs::OpenFile("Scene File (*.agescene)| *.agescene")};

    if (!filepath.empty()) {
      AGE_TRACE("Loading scene from \"{0}\"", filepath);
      CreateNewScene();

      SceneSerializer serializer{m_ActiveScene};
      serializer.Deserialize(filepath);

      m_SceneFilepath = filepath;
    }
  }

  void EditorLayer::OnEvent(Event& e) {
    Layer::OnEvent(e);
    m_EditorCamera.OnEvent(e);

    EventDispatcher dispatcher{e};
    dispatcher.Dispatch<KeyPressedEvent>(
        AGE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
  }

  void EditorLayer::SetDarkThemeColors() {
    auto& colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

    const ImVec4 interactable{0.2f, 0.205f, 0.21f, 1.0f};
    const ImVec4 interactableHovered{0.3f, 0.305f, 0.31f, 1.0f};
    const ImVec4 interactableActive{0.15f, 0.1505f, 0.151f, 1.0f};

    colors[ImGuiCol_Header] = interactable;
    colors[ImGuiCol_HeaderHovered] = interactableHovered;
    colors[ImGuiCol_HeaderActive] = interactableActive;

    colors[ImGuiCol_Button] = interactable;
    colors[ImGuiCol_ButtonHovered] = interactableHovered;
    colors[ImGuiCol_ButtonActive] = interactableActive;

    colors[ImGuiCol_FrameBg] = interactable;
    colors[ImGuiCol_FrameBgHovered] = interactableHovered;
    colors[ImGuiCol_FrameBgActive] = interactableActive;

    const ImVec4& tabBase{0.15f, 0.1505f, 0.151f, 1.0f};
    const ImVec4& tabHovered{0.38f, 0.3805f, 0.381f, 1.0f};
    const ImVec4& tabActive{0.28f, 0.2805f, 0.281f, 1.0f};

    colors[ImGuiCol_Tab] = tabBase;
    colors[ImGuiCol_TabHovered] = tabHovered;
    colors[ImGuiCol_TabActive] = tabActive;
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

    colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.95f, 0.1505f, 0.951f, 1.0f};
  }

  bool EditorLayer::OnKeyPressed(KeyPressedEvent& e) {
    bool handled{false};

    bool ctrlPressed = Input::IsKeyPressed(Key::LeftControl) ||
                       Input::IsKeyPressed(Key::RightControl);
    bool shiftPressed = Input::IsKeyPressed(Key::LeftShift) ||
                        Input::IsKeyPressed(Key::RightShift);
    switch (e.KeyCode()) {
      case Key::S: {
        if (ctrlPressed && shiftPressed) {
          SaveSceneAsDialog();
          handled = true;
        } else if (ctrlPressed) {
          SaveScene();
          handled = true;
        }
        break;
      }
      case Key::O: {
        if (ctrlPressed) {
          OpenSceneDialog();
          handled = true;
        }
        break;
      }
      case Key::N: {
        if (ctrlPressed) {
          CreateNewScene();
          handled = true;
        }
        break;
      }
      case Key::Q: {
        if (ctrlPressed) {
          m_GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
          handled = true;
        }
        break;
      }
      case Key::W: {
        if (ctrlPressed) {
          m_GizmoOperation = ImGuizmo::OPERATION::ROTATE;
          handled = true;
        }
        break;
      }
      case Key::E: {
        if (ctrlPressed) {
          m_GizmoOperation = ImGuizmo::OPERATION::SCALE;
          handled = true;
        }
        break;
      }
      case Key::L: {
        if (ctrlPressed) {
          m_GizmoMode = m_GizmoMode == ImGuizmo::MODE::LOCAL
                        ? ImGuizmo::MODE::WORLD
                        : ImGuizmo::MODE::LOCAL;
          handled = true;
        }
        break;
      }
      default:
        break;
    }

    return handled;
  }

} // namespace AGE

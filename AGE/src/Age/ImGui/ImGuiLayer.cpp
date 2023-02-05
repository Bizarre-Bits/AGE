//
// Created by alex on 05.09.22.
//

#include "agepch.h"

// TODO: must be moved to the platform specific code
#include "Platform/OpenGL/OpenGLPlatform.h"

#include "Age/ImGui/ImGuiLayer.h"

#include "Age/Core/Application.h"


namespace AGE {
  bool ImGuiLayer::IsInitialized{false};

  ImGuiLayer::ImGuiLayer(age_string_t name) : Layer(name) {}

  void ImGuiLayer::OnAttach() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      style.WindowRounding              = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    InitImGui();
  }

  void ImGuiLayer::OnDetach() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  void ImGuiLayer::Begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void ImGuiLayer::End() {
    ImGuiIO& io          = ImGui::GetIO();
    Window& window = Application::Instance()->Window();
    io.DisplaySize       = ImVec2((float)window.Width(), (float)window.Height());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
      GLFWwindow* backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
    }
  }

  void ImGuiLayer::InitImGui() {
    if (!IsInitialized) {
      auto window = (GLFWwindow*)Application::Instance()->Window().NativeWindow();
      ImGui_ImplGlfw_InitForOpenGL(window, true);
      ImGui_ImplOpenGL3_Init("#version 410");
      IsInitialized = true;
    }
  }
}// namespace AGE

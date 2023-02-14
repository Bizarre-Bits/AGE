//
// Created by alex on 2/5/23.
//

#include "OpenGLImGuiHandler.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

#include "Age/Core/Application.h"
#include "backends/imgui_impl_glfw.h"

namespace AGE {
  bool OpenGLImGuiHandler::s_IsInitialized{false};

  void OpenGLImGuiHandler::InitImGui() {
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

    if (!s_IsInitialized) {
      auto window = (GLFWwindow*)Application::Instance()->Window().NativeWindow();
      ImGui_ImplGlfw_InitForOpenGL(window, true);
      ImGui_ImplOpenGL3_Init("#version 430");
      s_IsInitialized = true;
    }
  }

  void OpenGLImGuiHandler::ShutDownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    s_IsInitialized = false;
  }

  void OpenGLImGuiHandler::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void OpenGLImGuiHandler::EndFrame() {
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
} // AGE

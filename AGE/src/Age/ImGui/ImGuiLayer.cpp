//
// Created by alex on 05.09.22.
//

#include "agepch.h"

#include "Age/ImGui/ImGuiLayer.h"

#include "imgui.h"

// Temporary. will be moved to the OpenGL platform code
#include "Platform/OpenGL/PlatformGL.h"
#include "backends/imgui_impl_opengl3.h"

#include "Age/Core/KeyCodes.h"
#include "Age/Core/MouseCodes.h"

#include "Age/Core/Application.h"


namespace AGE {
  ImGuiLayer::ImGuiLayer() : Layer("ImGui"), time_{0.0f} {}

  ImGuiLayer::~ImGuiLayer() {
  }

  void ImGuiLayer::OnAttach() {
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();

    Window* window = Application::Get()->Window();
    io.DisplaySize = ImVec2(window->Width(), window->Height());

    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    //        io.KeyMap[ImGuiKey_MouseLeft] = Mouse::ButtonLeft;

    ImGui_ImplOpenGL3_Init("#version 410");
  }

  void ImGuiLayer::OnDetach() {
    Layer::OnDetach();
  }

  void ImGuiLayer::OnUpdate() {
    ImGuiIO& io = ImGui::GetIO();

    auto time    = (float)glfwGetTime();
    io.DeltaTime = time_ > 0.0f ? (time - time_) : (1.0f / 60.0f);
    time_        = time;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static bool showDemo{false};
    ImGui::Begin("Debug");
    ImGui::Text("Window (%i, %i): %f FPS", (int)io.DisplaySize.x, (int)io.DisplaySize.y, 1.0f / io.DeltaTime);
    ImGui::Text("MousePos: %i, %i", (int)io.MousePos.x, (int)io.MousePos.y);
    if (ImGui::Button("Demo"))
      showDemo = !showDemo;
    ImGui::End();

    if (showDemo) {
      ImGui::ShowDemoWindow();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void ImGuiLayer::OnEvent(Event& e) {
    EventDispatcher d{e};
    d.Dispatch<MouseButtonPressedEvent>(AGE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
    d.Dispatch<MouseButtonReleasedEvent>(AGE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
    d.Dispatch<MouseMovedEvent>(AGE_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
    d.Dispatch<MouseScrolledEvent>(AGE_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
    d.Dispatch<WindowResizeEvent>(AGE_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
    d.Dispatch<KeyTypedEvent>(AGE_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
    d.Dispatch<KeyPressedEvent>(AGE_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
    d.Dispatch<KeyReleasedEvent>(AGE_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
  }

  bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e) {
    ImGuiIO& io = ImGui::GetIO();

    io.MouseDown[e.MouseButton()] = true;

    return false;
  }

  bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e) {
    ImGuiIO& io = ImGui::GetIO();

    io.MouseDown[e.MouseButton()] = false;

    return false;
  }

  bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e) {
    ImGuiIO& io = ImGui::GetIO();

    io.MousePos = ImVec2(e.X(), e.Y());

    return false;
  }

  bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e) {
    ImGuiIO& io = ImGui::GetIO();

    io.MouseWheel += e.YOffset();
    io.MouseWheelH += e.XOffset();

    return false;
  }

  bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e) {
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2((float)e.Width(), (float)e.Height());

    return false;
  }

  bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e) {
    ImGuiIO& io = ImGui::GetIO();
    int keyCode = e.GetKeyCode();

    if (keyCode > 0 && keyCode < 0x10000)
      io.AddInputCharacter(keyCode);

    return false;
  }

  bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e) {
    ImGuiIO& io = ImGui::GetIO();

    io.KeysDown[e.GetKeyCode()] = true;

    return false;
  }

  bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e) {
    ImGuiIO& io = ImGui::GetIO();

    io.KeysDown[e.GetKeyCode()] = true;

    return false;
  }
}// namespace AGE
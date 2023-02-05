//
// Created by a on 24.08.22.
//

#include "agepch.h"

#include "Application.h"

#include "Age/Debug/DebugLayer.h"
#include "Age/Renderer/Renderer.h"
#include "Age/Renderer/Renderer2D.h"
#include "Age/Renderer/ImGuiHandler.h"

namespace AGE {
  Application* Application::s_Instance{nullptr};

  Application::Application(const ApplicationSpecs& specs) : m_Running{true}, m_LayerStack() {
    AGE_PROFILE_FUNCTION();

    s_Instance = this;

    WindowProps windowProps{};
    windowProps.Width = specs.Width;
    windowProps.Height = specs.Height;
    windowProps.Title = specs.AppTitle;
    windowProps.IconPath = specs.AppIcon;

    m_Window = Window::Create(windowProps);
    m_Window->EventCallback(AGE_BIND_EVENT_FN(&Application::OnEvent));

    Renderer::Init();
    Renderer2D::Init();
    ImGuiHandler::InitImGui();

#ifdef DEBUG
    PushOverlay(new DebugLayer);
#endif
  }

  void Application::Run() {
    AGE_PROFILE_FUNCTION();

    m_Timer.Start();
    while (m_Running) {
      AGE_PROFILE_SCOPE("RunLoop");

      Timestep ts = m_Timer.DeltaTime();

      {
        AGE_PROFILE_SCOPE("LayerStack OnUpdate");

        for (Layer* layer: m_LayerStack)
          layer->OnUpdate(ts);
      }
      {
        AGE_PROFILE_SCOPE("LayerStack OnUiRender");
        ImGuiHandler::BeginFrame();
        for (Layer* layer: m_LayerStack)
          layer->OnUiRender(ts);
        ImGuiHandler::EndFrame();
      }

      m_Window->OnUpdate();

    }

    ImGuiHandler::ShutDownImGui();
  }

  void Application::OnEvent(Event& e) {
    AGE_PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(AGE_BIND_EVENT_FN(&Application::OnWindowClose));

    for (auto it{m_LayerStack.end()}; it != m_LayerStack.begin();) {
      (*(--it))->OnEvent(e);
      if (e.Handled) break;
    }
  }

  bool Application::OnWindowClose(WindowCloseEvent& e) {
    AGE_PROFILE_FUNCTION();

    m_Running = false;
    return true;
  }

  void Application::PushLayer(Layer* layer) {
    AGE_PROFILE_FUNCTION();

    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
  }

  void Application::PushOverlay(Layer* layer) {
    AGE_PROFILE_FUNCTION();

    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
  }

  Application* Application::Instance() {
    AGE_PROFILE_FUNCTION();

    AGE_ASSERT(s_Instance);
    return s_Instance;
  }

  Window& Application::Window() {
    AGE_PROFILE_FUNCTION();

    return *m_Window;
  }

  Timestep Application::Uptime() {
    AGE_PROFILE_FUNCTION();

    return m_Timer.Uptime();
  }
}// namespace AGE

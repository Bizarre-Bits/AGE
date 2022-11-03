//
// Created by a on 24.08.22.
//

#include "agepch.h"

#include "Application.h"

#include "Age/Debug/Assert.h"
#include "Age/Debug/DebugLayer.h"
#include "Age/ImGui/ImGuiLayer.h"
#include "Age/Renderer/Renderer.h"
#include "Age/Renderer/Renderer2D.h"

namespace AGE {
  Application* Application::s_Instance{nullptr};

  Application::Application() : m_Running{true}, m_LayerStack() {
    s_Instance = this;

    m_Window.reset(Window::Create());
    m_Window->EventCallback(AGE_BIND_EVENT_FN(&Application::OnEvent));

    Renderer::Init();
    Renderer2D::Init();

#ifdef DEBUG
    PushOverlay(new DebugLayer);
#endif
  }

  void Application::Run() {
    m_Timer.Start();
    while (m_Running) {
      Timestep ts = m_Timer.DeltaTime();

      // TODO: That looks strange, should figure out a better solution.
      if (ImGuiLayer::IsInitialized)
        ImGuiLayer::Begin();

      for (Layer* layer: m_LayerStack)
        layer->OnUpdate(ts);

      if (ImGuiLayer::IsInitialized)
        ImGuiLayer::End();

      m_Window->OnUpdate();

    }
  }

  void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(AGE_BIND_EVENT_FN(&Application::OnWindowClose));

    for (auto it{m_LayerStack.end()}; it != m_LayerStack.begin();) {
      (*(--it))->OnEvent(e);
      if (e.Handled) break;
    }
  }

  bool Application::OnWindowClose(WindowCloseEvent& e) {
    m_Running = false;
    return true;
  }

  void Application::PushLayer(Layer* layer) {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
  }

  void Application::PushOverlay(Layer* layer) {
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
  }

  Application* Application::Instance() {
    AGE_ASSERT(s_Instance);
    return s_Instance;
  }

  Scope<Window>& Application::Window() {
    return m_Window;
  }

  Timestep Application::Uptime() {
    return m_Timer.Uptime();
  }
}// namespace AGE
//
// Created by a on 24.08.22.
//

#include "agepch.h"

#include "Application.h"

#include "Age/Debug/Assert.h"

namespace AGE {
  Application* Application::s_Instance{nullptr};

  Application::Application() {
    s_Instance = this;

    m_Window = Window::Create();
    m_Window->EventCallback(AGE_BIND_EVENT_FN(Application::OnEvent));
  }

  Application::~Application() {
    delete m_Window;
  }

  void Application::Run() {
    while (m_Running) {
      for (Layer* layer: m_LayerStack)
        layer->OnUpdate();

      m_Window->OnUpdate();
    }
  }

  void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(AGE_BIND_EVENT_FN(Application::OnWindowClose));

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

  Window* Application::Window() {
    return m_Window;
  }
}// namespace AGE
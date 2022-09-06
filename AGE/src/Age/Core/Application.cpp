//
// Created by a on 24.08.22.
//

#include "agepch.h"

#include "Application.h"

#include "Age/Debug/Assert.h"

namespace AGE {
  Application* Application::s_Application{nullptr};

  Application::Application() {
    s_Application = this;

    window_ = Window::Create();
    window_->EventCallback(AGE_BIND_EVENT_FN(Application::OnEvent));
  }

  Application::~Application() {
    delete window_;
  }

  void Application::Run() {
    while (running_) {
      for (Layer* layer: layerStack_)
        layer->OnUpdate();

      window_->OnUpdate();
    }
  }

  void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(AGE_BIND_EVENT_FN(Application::OnWindowClose));

    for (auto it{layerStack_.end()}; it != layerStack_.begin();) {
      (*(--it))->OnEvent(e);
      if (e.Handled) break;
    }
  }

  bool Application::OnWindowClose(WindowCloseEvent& e) {
    running_ = false;
    return true;
  }

  void Application::PushLayer(Layer* layer) {
    layerStack_.PushLayer(layer);
    layer->OnAttach();
  }

  void Application::PushOverlay(Layer* layer) {
    layerStack_.PushOverlay(layer);
    layer->OnAttach();
  }

  Application* Application::Get() {
    AGE_ASSERT(s_Application);
    return s_Application;
  }

  Window* Application::Window() {
    return window_;
  }
}// namespace AGE
//
// Created by a on 24.08.22.
//

#ifndef AGE_APPLICATION_H
#define AGE_APPLICATION_H

#include "Core.h"

#include "Age/Events/Event.h"
#include "Age/Events/WindowEvent.h"
#include "LayerStack.h"
#include "Window.h"
#include "Timer.h"

namespace AGE {
  class Application {
  public:
    Application();
    virtual ~Application() = default;

    virtual void Run();

    virtual void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    AGE::Window& Window();
    Timestep Uptime();

    static Application* Instance();

  private:
    bool OnWindowClose(WindowCloseEvent& e);

  private:
    Scope <AGE::Window> m_Window;

    bool       m_Running;
    LayerStack m_LayerStack;
    Timer      m_Timer;

    static Application* s_Instance;
  };

  // To be defined in CLIENT
  Application* CreateApplication();
}// namespace AGE

#endif//AGE_APPLICATION_H

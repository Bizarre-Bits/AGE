//
// Created by a on 24.08.22.
//

#include "agepch.h"

#include "Application.h"

#include "Age/Debug/Assert.h"
#include "Age/Debug/DebugLayer.h"
#include "Age/ImGui/ImGuiLayer.h"

// TODO: Temp
#include "Platform/OpenGL/PlatformGL.h"

namespace AGE {
  Application* Application::s_Instance{nullptr};

  Application::Application() {
    s_Instance = this;

    m_Window = Window::Create();
    m_Window->EventCallback(AGE_BIND_EVENT_FN(Application::OnEvent));

#ifdef DEBUG
    PushOverlay(new DebugLayer);
#endif

    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

    float vertices[3 * 3]{
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glGenBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

    unsigned int indices[3]{0, 1, 2};
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  }

  Application::~Application() {
    delete m_Window;
  }

  void Application::Run() {
    while (m_Running) {
      m_Window->Clear();

      glBindVertexArray(m_VertexArray);
      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

      // TODO: That looks strange, should figure out a better solution.
      if (ImGuiLayer::IsInitialized)
        ImGuiLayer::Begin();

      for (Layer* layer: m_LayerStack)
        layer->OnUpdate();

      if (ImGuiLayer::IsInitialized)
        ImGuiLayer::End();

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
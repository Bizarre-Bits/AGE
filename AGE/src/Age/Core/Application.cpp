//
// Created by a on 24.08.22.
//

#include "agepch.h"

#include "Application.h"

#include "Age/Debug/Assert.h"
#include "Age/Debug/DebugLayer.h"
#include "Age/ImGui/ImGuiLayer.h"

// TODO: Temp
#include "Platform/OpenGL/OpenGLPlatform.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

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

    float vertices[3 * 7]{
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
    };
    m_VertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float));

    BufferLayout layout{
        {"a_Position", ShaderDataType::Float3},
        {"a_Color", ShaderDataType::Float4}
    };
    m_VertexBuffer->SetLayout(layout);

    uint32_t index{0};
    for (const BufferElement& element: m_VertexBuffer->Layout()) {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(
          index, element.ComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type),
          element.Normalized ? GL_TRUE : GL_FALSE,
          m_VertexBuffer->Layout().Stride(), (void*)element.Offset
      );
      index++;
    }

    unsigned int indices[3]{0, 1, 2};
    m_IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

    age_string_t vertexSrc{R"(
       #version 330 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;

        out vec4 v_Color;

        void main() {
          gl_Position = vec4(a_Position, 1.0f);
          v_Color = a_Color;
        }
    )"};

    age_string_t fragmentSrc{R"(
        #version 330 core

        layout(location = 0) out vec4 color;
        in vec4 v_Color;

        void main() {
          color = v_Color;
        }
    )"};

    m_Shader = new OpenGLShader(vertexSrc, fragmentSrc);
  }

  Application::~Application() {
    delete m_Window;
    delete m_Shader;
  }

  void Application::Run() {
    while (m_Running) {
      m_Window->Clear();

      AGE_CORE_ASSERT(m_Shader);
      m_Shader->Bind();

      glBindVertexArray(m_VertexArray);
      glDrawElements(GL_TRIANGLES, m_IndexBuffer->Count(), GL_UNSIGNED_INT, nullptr);

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
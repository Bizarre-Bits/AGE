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

    auto layout = BufferLayout{
        {"aPos", ShaderDataType::Float3},
        {"aCol", ShaderDataType::Float4}
    };

    {
      m_TriangleVA = VertexArray::Create();

      float vertices[3 * 7]{
          0.0f, 0.5f, 0.0f, 0.8f, 0.2f, 0.2f, 1.0f,
          -0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.2f, 1.0f,
          0.5f, -0.5f, 0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
      };

      auto VB = VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float));

      VB->SetLayout(
          layout
      );

      m_TriangleVA->AddVertexBuffer(VB);

      uint32_t indices[3]{0, 1, 2};

      auto IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

      m_TriangleVA->SetIndexBuffer(IB);
    }

    {
      m_SquareVA = VertexArray::Create();

      float vertices[4 * 7]{
          -0.6f, 0.6f, 0.0f, 0.1f, 0.6f, 0.1f, 1.0f,
          -0.6f, -0.6f, 0.0f, 0.1f, 0.1f, 0.6f, 1.0f,
          0.6f, -0.6f, 0.0f, 0.6f, 0.1f, 0.1f, 1.0f,
          0.6f, 0.6f, 0.0f, 0.1f, 0.6f, 0.1f, 1.0f,
      };

      auto VB = VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float));

      VB->SetLayout(
          layout
      );

      m_SquareVA->AddVertexBuffer(VB);
//
      uint32_t indices[6]{0, 2, 1, 0, 3, 2};

      auto IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

      m_SquareVA->SetIndexBuffer(IB);
    }

    AGE_CORE_ASSERT(m_TriangleVA != m_SquareVA);
    AGE_CORE_ASSERT(m_TriangleVA->VertexBuffers()[0] != m_SquareVA->VertexBuffers()[0])

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

//      Renderer::BeginScene();
//
//      Renderer::Submit();
//
//      Renderer::EndScene();
//
//      Renderer::Flush();

      AGE_CORE_ASSERT(m_Shader);
      m_Shader->Bind();

      m_SquareVA->Bind();
      glDrawElements(GL_TRIANGLES, m_SquareVA->IndexBuffer()->Count(), GL_UNSIGNED_INT, (void*)0);

      m_TriangleVA->Bind();
      glDrawElements(GL_TRIANGLES, m_TriangleVA->IndexBuffer()->Count(), GL_UNSIGNED_INT, (void*)0);



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
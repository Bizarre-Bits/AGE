//
// Created by alex on 04.10.22.
//

#include <glm/gtc/matrix_transform.hpp>

#include "Sandbox.h"

AGE::Application* AGE::CreateApplication() {
  return new Sandbox;
}

Sandbox::Sandbox() : AGE::Application() {
  PushLayer(new SandboxLayer);
}

Sandbox::~Sandbox() {}

// Sandbox Layer

void SandboxLayer::OnUpdate(AGE::Timestep ts) {
  glm::vec3 cameraPos = m_Camera.Position();

  if (AGE::Input::IsKeyPressed(AGE::Key::W)) {
    cameraPos.y += m_CameraSpeed * ts.Seconds();
  }
  if (AGE::Input::IsKeyPressed(AGE::Key::S)) {
    cameraPos.y -= m_CameraSpeed * ts.Seconds();
  }
  if (AGE::Input::IsKeyPressed(AGE::Key::D)) {
    cameraPos.x += m_CameraSpeed * ts.Seconds();
  }
  if (AGE::Input::IsKeyPressed(AGE::Key::A)) {
    cameraPos.x -= m_CameraSpeed * ts.Seconds();
  }
  m_Camera.SetPosition(cameraPos);

  if (AGE::Input::IsKeyPressed(AGE::Key::Up)) {
    m_TrianglePos.y += m_TriangleSpeed * ts;
  }
  if (AGE::Input::IsKeyPressed(AGE::Key::Down)) {
    m_TrianglePos.y -= m_TriangleSpeed * ts;
  }
  if (AGE::Input::IsKeyPressed(AGE::Key::Right)) {
    m_TrianglePos.x += m_TriangleSpeed * ts;
  }
  if (AGE::Input::IsKeyPressed(AGE::Key::Left)) {
    m_TrianglePos.x -= m_TriangleSpeed * ts;
  }

  AGE::RenderCommand::Clear();
  AGE::Renderer::BeginScene(m_Camera);


  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 20; x++) {
      glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
      glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
      AGE::Renderer::Submit(m_SquareVA, m_Shader, transform);
    }
  }

  AGE::Renderer::Submit(
      m_TriangleVA, m_Shader,
      glm::translate(glm::mat4(1.0f), m_TrianglePos)
      * glm::rotate(
          glm::mat4(1.0f), glm::radians(m_TriangleRotation),
          glm::vec3(0.0f, 0.0f, 1.0f))
  );
}

SandboxLayer::~SandboxLayer() {
  delete m_Shader;
  delete m_TriangleVA;
  delete m_SquareVA;
}

SandboxLayer::SandboxLayer() : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
                               m_CameraSpeed{5.0f},
                               m_TriangleSpeed(2.0f),
                               m_TrianglePos(0.0f) {
  AGE::RenderCommand::SetClearColor(glm::vec4{0.2f, 0.2f, 0.2f, 1.0f});

  auto layout = AGE::BufferLayout{
      {"aPos", AGE::ShaderDataType::Float3},
      {"aCol", AGE::ShaderDataType::Float4}
  };

  {
    m_TriangleVA = AGE::VertexArray::Create();

    float vertices[3 * 7]{
        0.0f, 0.5f, 0.0f,
        0.8f, 0.2f, 0.2f, 1.0f,

        -0.5f, -0.5f, 0.0f,
        0.2f, 0.8f, 0.2f, 1.0f,

        0.5f, -0.5f, 0.0f,
        0.2f, 0.2f, 0.8f, 1.0f,
    };

    auto VB = AGE::VertexBuffer::Create(
        vertices, sizeof(vertices) / sizeof(float));

    VB->SetLayout(
        layout
    );

    m_TriangleVA->AddVertexBuffer(VB);

    uint32_t indices[3]{0, 1, 2};

    auto IB = AGE::IndexBuffer::Create(
        indices, sizeof(indices) / sizeof(uint32_t));

    m_TriangleVA->SetIndexBuffer(IB);
  }

  {
    m_SquareVA = AGE::VertexArray::Create();

    float vertices[4 * 7]{
        -0.5f, 0.5f, 0.0f,
        0.1f, 0.6f, 0.1f, 1.0f,

        -0.5f, -0.5f, 0.0f,
        0.1f, 0.1f, 0.6f, 1.0f,

        0.5f, -0.5f, 0.0f,
        0.6f, 0.1f, 0.1f, 1.0f,

        0.5f, 0.5f, 0.0f,
        0.1f, 0.6f, 0.1f, 1.0f,
    };

    auto VB = AGE::VertexBuffer::Create(
        vertices, sizeof(vertices) / sizeof(float));

    VB->SetLayout(
        layout
    );

    m_SquareVA->AddVertexBuffer(VB);
//
    uint32_t indices[6]{0, 2, 1, 0, 3, 2};

    auto IB = AGE::IndexBuffer::Create(
        indices, sizeof(indices) / sizeof(uint32_t));

    m_SquareVA->SetIndexBuffer(IB);
  }

  age_string_t vertexSrc{R"(
       #version 330 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        out vec4 v_Color;

        void main() {
          gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0f);
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


  m_Shader = AGE::Shader::Create(vertexSrc, fragmentSrc);
}

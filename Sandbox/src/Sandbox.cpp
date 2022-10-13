//
// Created by alex on 04.10.22.
//

#include <glm/gtc/matrix_transform.hpp>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Sandbox.h"
#include "glm/gtc/type_ptr.hpp"

AGE::Application* AGE::CreateApplication() {
  return new Sandbox;
}

Sandbox::Sandbox() : AGE::Application() {
  PushLayer(new SandboxLayer);
}

Sandbox::~Sandbox() {}

// Sandbox Layer

SandboxLayer::SandboxLayer() : m_Camera(-1.6, 1.6, -0.9, 0.9) {
  m_SquareVA = AGE::Ref<AGE::VertexArray>(AGE::VertexArray::Create());

  float vertices[4 * 5]{
      -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.0f, 1.0f, 1.0f
  };

  AGE::Ref<AGE::VertexBuffer> vb = AGE::Ref<AGE::VertexBuffer>(
      AGE::VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float)));

  AGE::BufferLayout layout{
      {"a_Pos",      AGE::ShaderDataType::Float3},
      {"a_TexCoord", AGE::ShaderDataType::Float2}
  };

  vb->SetLayout(layout);

  uint32_t indices[6]{0, 1, 2, 0, 2, 3};

  AGE::Ref<AGE::IndexBuffer> ib = AGE::Ref<AGE::IndexBuffer>(
      AGE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

  m_SquareVA->AddVertexBuffer(vb);
  m_SquareVA->SetIndexBuffer(ib);

  //Shaders

  age_string_t flatVert{R"(
    #version 330 core

    layout (location = 0) in vec3 a_Pos;

    uniform mat4 u_ViewProjection;
    uniform mat4 u_Transform;

    void main() {
      gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0f);
    }
  )"};

  age_string_t flatFrag{R"(
    #version 330 core
    out vec4 color;

    uniform vec3 u_Color;

    void main() {
      color = vec4(u_Color, 0.0);
    }
  )"};

  m_FlatColorShader = AGE::Ref<AGE::Shader>(AGE::Shader::Create(flatVert, flatFrag));

  age_string_t texVert{R"(
    #version 330 core

    layout(location = 0) in vec3 a_Pos;
    layout(location = 1) in vec2 a_TexCoord;

    uniform mat4 u_ViewProjection;
    uniform mat4 u_Transform;

    out vec2 v_TexCoord;

    void main() {
      v_TexCoord = a_TexCoord;
      gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0f);
    }
  )"};

  age_string_t texFrag{R"(
    #version 330 core

    in vec2 v_TexCoord;
    out vec4 f_Color;

    uniform sampler2D u_Texture;

    void main() {
      f_Color = texture(u_Texture, v_TexCoord);
    }
  )"};

  m_TextureShader = AGE::Ref<AGE::Shader>(AGE::Shader::Create(texVert, texFrag));

  m_Texture = AGE::Texture2D::Create("assets/textures/Checkerboard.png");
  m_TextureShader->Bind();
  m_Texture->Bind();
  std::dynamic_pointer_cast<AGE::OpenGLShader>(m_FlatColorShader)->UploadUniformInt(
      "u_Texture", 0
  );
}

SandboxLayer::~SandboxLayer() {

}

void SandboxLayer::OnUpdate(AGE::Timestep ts) {
  AGE::Renderer::BeginScene(m_Camera);

  AGE::RenderCommand::Clear();


  glm::vec3 redColor(0.6f, 0.2f, 0.3f);
  glm::vec3 blueColor(0.3f, 0.2f, 0.6f);

  glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));

  glm::mat4 redTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, -0.25f, 0.0f))
                           * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));

  for (int y = 0; y < 20; y++) {
    for (int x = 0; x < 20; x++) {
      glm::vec3 pos(x * 0.06f, y * 0.06f, 0.0f);
      glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
      std::dynamic_pointer_cast<AGE::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3(
          "u_Color", blueColor
      );
      AGE::Renderer::Submit(m_SquareVA, m_FlatColorShader, transform);
    }
  }

  AGE::Renderer::Submit(m_SquareVA, m_TextureShader, redTransform);

  AGE::Renderer::EndScene();
}

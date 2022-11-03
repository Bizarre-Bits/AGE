//
// Created by alex on 18.10.22.
//

#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

namespace AGE {
  struct Renderer2DStorage {
    Ref<VertexArray> QuadVertexArray;
    Ref<Shader>      FlatColorShader;
  };

  static Renderer2DStorage* s_Storage;

  void Renderer2D::Init() {
    s_Storage = new Renderer2DStorage;

    s_Storage->FlatColorShader = AGE::Shader::Create("assets/shaders/FlatColor.glsl");


    s_Storage->QuadVertexArray = AGE::VertexArray::Create();

    float vertices[4 * 5]{
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    auto VBO = AGE::VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float));

    AGE::BufferLayout layout{
        {"a_Pos", AGE::ShaderDataType::Float3},
    };

    VBO->SetLayout(layout);

    s_Storage->QuadVertexArray->AddVertexBuffer(VBO);

    uint32_t indices[6]{0, 1, 2, 0, 2, 3};

    auto IB = AGE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

    s_Storage->QuadVertexArray->SetIndexBuffer(IB);
  }

  void Renderer2D::ShutDown() {
    delete s_Storage;
  }

  void Renderer2D::BeginScene(const OrthographicCamera& camera) {
    s_Storage->FlatColorShader->Bind();
    s_Storage->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
  }

  void Renderer2D::EndScene() {

  }

  void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
    s_Storage->FlatColorShader->Bind();
    s_Storage->QuadVertexArray->Bind();

    glm::mat4 transform = glm::translate(glm::mat4{1.0f}, {pos, 0.0f})
                          * glm::scale(glm::mat4{1.0f}, {size, 1.0f});
    s_Storage->FlatColorShader->SetFloat4("u_Color", color);
    s_Storage->FlatColorShader->SetMat4("u_Transform", transform);

    RenderCommand::DrawIndexed(s_Storage->QuadVertexArray);
  }
} // AGE
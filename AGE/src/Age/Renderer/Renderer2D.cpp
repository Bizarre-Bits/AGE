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
    Ref<Shader>      Shader2D;
    Ref<Texture2D>   UnitTexture;
  };

  static Renderer2DStorage* s_Storage;

  void Renderer2D::Init() {
    s_Storage = new Renderer2DStorage;

    s_Storage->Shader2D    = AGE::Shader::Create("assets/shaders/2dRendererShader.glsl");
    s_Storage->UnitTexture = AGE::Texture2D::Create(1, 1);

    uint8_t data[]{255, 255, 255, 255};
    s_Storage->UnitTexture->SetData(data, sizeof(data));

    s_Storage->QuadVertexArray = AGE::VertexArray::Create();

    float vertices[4 * 5]{-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,
                          -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 1.0f};

    auto VBO = AGE::VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float));

    AGE::BufferLayout layout{{"a_Pos",      AGE::ShaderDataType::Float3},
                             {"a_TexCoord", AGE::ShaderDataType::Float2}};

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
    s_Storage->Shader2D->Bind();
    s_Storage->Shader2D->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
  }

  void Renderer2D::EndScene() {

  }

  void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad(glm::vec3{pos, 0.0f}, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
    DrawQuad(pos, size, nullptr, color);
  }

  void Renderer2D::DrawQuad(
      const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture
  ) {
    DrawQuad(glm::vec3(pos, 0.0f), size, texture);
  }

  void Renderer2D::DrawQuad(
      const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture
  ) {
    DrawQuad(pos, size, texture, glm::vec4(1.0f));
  }

  void Renderer2D::DrawQuad(
      const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
      const glm::vec4& color
  ) {
    DrawQuad(glm::vec3(pos, 0.0f), size, texture, color);
  }

  void Renderer2D::DrawQuad(
      const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
      const glm::vec4& color
  ) {
    s_Storage->Shader2D->Bind();

    glm::mat4 transform = glm::translate(glm::mat4{1.0f}, pos)
                          * glm::scale(glm::mat4{1.0f}, {size, 1.0f});

    bool isTexCorrect = false;

    if (!texture) {
      const Ref<Texture2D>& unitTex = s_Storage->UnitTexture;
      unitTex->Bind();
      s_Storage->Shader2D->SetInt("u_Texture", (int)unitTex->Slot());
      isTexCorrect = true;
    } else {
      texture->Bind();
      s_Storage->Shader2D->SetInt("u_Texture", (int)texture->Slot());
      isTexCorrect = texture->IsCorrect();
    }

    if (isTexCorrect) {
      s_Storage->Shader2D->SetFloat4("u_Color", color);
    } else {
      s_Storage->Shader2D->SetFloat4("u_Color", glm::vec4{1.0f});
    }

    s_Storage->Shader2D->SetMat4("u_Transform", transform);

    RenderCommand::DrawIndexed(s_Storage->QuadVertexArray);
  }
} // AGE
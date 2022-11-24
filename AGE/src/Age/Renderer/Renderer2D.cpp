//
// Created by alex on 18.10.22.
//

#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Age/Debug/Instrumentor.h"

namespace AGE {
  struct QuadVertex {
    glm::vec3 Pos;
    glm::vec4 Color;
    glm::vec2 TexCoord;
  };

  struct Renderer2DStorage {
    const uint32_t MaxQuads    = 10'000;
    const uint32_t MaxVertices = MaxQuads * 4;
    const uint32_t MaxIndices  = MaxQuads * 6;

    Ref<VertexArray>  QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<Shader>       Shader2D;
    Ref<Texture2D>    UnitTexture;

    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr  = nullptr;

    uint32_t QuadIndexCount = 0;
  };

  static Renderer2DStorage s_Data;

  void Renderer2D::Init() {
    AGE_PROFILE_FUNCTION();

    s_Data.Shader2D    = AGE::Shader::Create("assets/shaders/2dRendererShader.glsl");
    s_Data.UnitTexture = AGE::Texture2D::Create(1, 1);

    uint8_t data[]{255, 255, 255, 255};
    s_Data.UnitTexture->SetData(data, sizeof(data));

    s_Data.QuadVertexArray = AGE::VertexArray::Create();


    s_Data.QuadVertexBuffer = VertexBuffer::Create(4 * sizeof(QuadVertex));
    AGE::BufferLayout layout{{"a_Pos",      AGE::ShaderDataType::Float3},
                             {"a_Color",    AGE::ShaderDataType::Float4},
                             {"a_TexCoord", AGE::ShaderDataType::Float2}};
    s_Data.QuadVertexBuffer->SetLayout(layout);
    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

    auto* indices = new uint32_t[s_Data.MaxIndices];

    for (uint32_t i{0}, offset{0}; i < s_Data.MaxIndices; i += 6, offset += 4) {
      indices[i + 0] = offset + 0;
      indices[i + 1] = offset + 1;
      indices[i + 2] = offset + 2;

      indices[i + 3] = offset + 0;
      indices[i + 4] = offset + 3;
      indices[i + 5] = offset + 2;
    }
    auto IB = AGE::IndexBuffer::Create(indices, s_Data.MaxIndices);
    delete[] indices;

    s_Data.QuadVertexArray->SetIndexBuffer(IB);

    s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
  }

  void Renderer2D::ShutDown() {
    AGE_PROFILE_FUNCTION();

  }

  void Renderer2D::BeginScene(const OrthographicCamera& camera) {
    AGE_PROFILE_FUNCTION();

    s_Data.Shader2D->Bind();
    s_Data.Shader2D->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

    StartBatch();
  }

  void Renderer2D::EndScene() {
    AGE_PROFILE_FUNCTION();

    Flush();
  }

  void Renderer2D::StartBatch() {
    s_Data.QuadIndexCount      = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
  }

  void Renderer2D::NextBatch() {
    Flush();
    StartBatch();
  }

  void Renderer2D::Flush() {
    AGE_PROFILE_FUNCTION();

    auto dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr
                               - (uint8_t*)s_Data.QuadVertexBufferBase);
    s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

    s_Data.Shader2D->Bind();
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

  }

  void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
    AGE_PROFILE_FUNCTION();

    DrawQuad(glm::vec3{pos, 0.0f}, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
    AGE_PROFILE_FUNCTION();

    DrawQuad(pos, size, nullptr, color);
  }

  void Renderer2D::DrawQuad(
      const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture
  ) {
    AGE_PROFILE_FUNCTION();

    DrawQuad(glm::vec3(pos, 0.0f), size, texture);
  }

  void Renderer2D::DrawQuad(
      const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture
  ) {
    AGE_PROFILE_FUNCTION();

    DrawQuad(pos, size, texture, glm::vec4(1.0f));
  }

  void Renderer2D::DrawQuad(
      const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
      const glm::vec4& color
  ) {
    AGE_PROFILE_FUNCTION();

    DrawQuad(glm::vec3(pos, 0.0f), size, texture, color);
  }

  void Renderer2D::DrawQuad(
      const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
      const glm::vec4& color
  ) {
    AGE_PROFILE_FUNCTION();

    if(s_Data.QuadIndexCount >= s_Data.MaxIndices)
      NextBatch();

    glm::vec2 halfSize{size * 0.5f};

    *s_Data.QuadVertexBufferPtr = QuadVertex{
        {pos.x - halfSize.x, pos.y + halfSize.y, pos.z},
        color,
        {0.0f, 0.0f}
    };
    s_Data.QuadVertexBufferPtr++;

    *s_Data.QuadVertexBufferPtr = QuadVertex{
        {pos.x + halfSize.x, pos.y + halfSize.y, pos.z},
        color,
        {1.0f, 0.0f}
    };
    s_Data.QuadVertexBufferPtr++;

    *s_Data.QuadVertexBufferPtr = QuadVertex{
        {pos.x + halfSize.x, pos.y - halfSize.y, pos.z},
        color,
        {1.0f, 1.0f}
    };
    s_Data.QuadVertexBufferPtr++;

    *s_Data.QuadVertexBufferPtr = QuadVertex{
        {pos.x - halfSize.x, pos.y - halfSize.y, pos.z},
        color,
        {0.0f, 1.0f}
    };
    s_Data.QuadVertexBufferPtr++;

    s_Data.QuadIndexCount += 6;

  }
} // AGE

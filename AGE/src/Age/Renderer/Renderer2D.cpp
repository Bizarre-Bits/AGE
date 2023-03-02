//
// Created by alex on 18.10.22.
//

#include "agepch.h"

#include "RenderCommand.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "VertexArray.h"

namespace AGE {
  struct QuadVertex {
    glm::vec3 Pos;
    glm::vec4 Color;
    glm::vec2 TexCoord;
    float TexIndex;
    float TilingFactor;
    int EntityID;
  };

  struct Renderer2DData {
    static constexpr uint32_t MaxQuads = 10'000;
    static constexpr uint32_t MaxVertices = MaxQuads * 4;
    static constexpr uint32_t MaxIndices = MaxQuads * 6;
    static constexpr uint32_t MaxTextureSlots = 32;

    Ref <VertexArray> QuadVertexArray;
    Ref <VertexBuffer> QuadVertexBuffer;
    Ref <Shader> Shader2D;
    Ref <Texture2D> UnitTexture;

    QuadVertex* QuadVertexBufferBase = nullptr;
    QuadVertex* QuadVertexBufferPtr = nullptr;

    uint32_t QuadIndexCount = 0;

    std::array<Ref < Texture2D>, MaxTextureSlots> TextureSlots{ UnitTexture };
    uint32_t TextureSlotIndex = 1;//0 - Unit Texture;

    glm::vec4 QuadVertexPositions[4];

    Renderer2D::Statistics Stats;
  };

  static Renderer2DData s_Data;

  void Renderer2D::Init() {
    AGE_PROFILE_FUNCTION();

    s_Data.Shader2D = AGE::Shader::Create("assets/core_assets/shaders/2dRendererShader.glsl");
    s_Data.UnitTexture = AGE::Texture2D::Create(1, 1);

    uint8_t data[]{255, 255, 255, 255};
    s_Data.UnitTexture->SetData(data, sizeof(data));

    s_Data.QuadVertexArray = AGE::VertexArray::Create();


    s_Data.QuadVertexBuffer = VertexBuffer::Create(4 * sizeof(QuadVertex));
    AGE::BufferLayout layout{{"a_Pos",           AGE::ShaderDataType::Float3},
                             {"a_Color",         AGE::ShaderDataType::Float4},
                             {"a_TexCoord",      AGE::ShaderDataType::Float2},
                             {"a_TexIndex",      AGE::ShaderDataType::Float},
                             {"a_TillingFactor", AGE::ShaderDataType::Float},
                             {"a_EntityID",      AGE::ShaderDataType::Int}};

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
    s_Data.TextureSlots[0] = s_Data.UnitTexture;

    int samplers[Renderer2DData::MaxTextureSlots];
    for (int i{0}; i < Renderer2DData::MaxTextureSlots; i++)
      samplers[i] = i;

    s_Data.Shader2D->Bind();
    s_Data.Shader2D->SetIntArray("u_Textures", samplers, Renderer2DData::MaxTextureSlots);

    s_Data.QuadVertexPositions[0] = {-0.5f, 0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[1] = {0.5f, 0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[2] = {0.5f, -0.5f, 0.0f, 1.0f};
    s_Data.QuadVertexPositions[3] = {-0.5f, -0.5f, 0.0f, 1.0f};
  }

  void Renderer2D::ShutDown() {
    AGE_PROFILE_FUNCTION();
  }

  void Renderer2D::BeginScene(const OrthographicCamera& camera) {
    AGE_PROFILE_FUNCTION();

    s_Data.Shader2D->Bind();
    s_Data.Shader2D->SetMat4("u_ViewProjection", camera.ViewProjectionMatrix());

    StartBatch();
  }

  void Renderer2D::BeginScene(const EditorCamera& camera) {
    AGE_PROFILE_FUNCTION();

    s_Data.Shader2D->Bind();
    s_Data.Shader2D->SetMat4("u_ViewProjection", camera.ViewProjection());

    StartBatch();
  }

  void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
    AGE_PROFILE_FUNCTION();

    glm::mat4 viewProj = camera.Projection() * glm::inverse(transform);

    s_Data.Shader2D->Bind();
    s_Data.Shader2D->SetMat4("u_ViewProjection", viewProj);

    StartBatch();
  }

  void Renderer2D::EndScene() {
    AGE_PROFILE_FUNCTION();

    Flush();
  }

  void Renderer2D::StartBatch() {
    s_Data.QuadIndexCount = 0;
    s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

    s_Data.TextureSlotIndex = 1;
  }

  void Renderer2D::NextBatch() {
    Flush();
    StartBatch();
  }

  void Renderer2D::Flush() {
    AGE_PROFILE_FUNCTION();

    auto dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr
                               - (uint8_t*)s_Data.QuadVertexBufferBase);
    if (dataSize == 0)
      return;

    s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

    s_Data.Shader2D->Bind();
    for (uint32_t i{0}; i < s_Data.TextureSlotIndex; i++) {
      s_Data.TextureSlots[i]->Bind(i);
    }
    RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);

    s_Data.Stats.DrawCalls++;
  }

#pragma region DrawQuad

  // /////////////////////////////////////////////////////////////////////////////////////////////////
  //                                        DRAW QUAD                                               //
  // /////////////////////////////////////////////////////////////////////////////////////////////////

  void Renderer2D::DrawQuad(const Renderer2D::QuadData& data) {
    constexpr uint32_t quadVertexCount = 4;
    constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f},
                                           {1.0f, 0.0f},
                                           {1.0f, 0.0f},
                                           {1.0f, 1.0f}};

    AGE_CORE_ASSERT(!(data.SubTexture && data.Texture),
                    "There cannot be texture and subtexture provided to the DrawQuad() at the same time");

    int textureIndex = 0.0f;
    glm::vec4 vertexColor{data.Color};
    if (data.Texture != nullptr) {
      textureIndex = FindTextureIndex(data.Texture);
      vertexColor = data.Texture->IsCorrect() ? vertexColor : glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
    } else if (data.SubTexture != nullptr) {
      textureIndex = FindTextureIndex(data.SubTexture->GetTexture());
      vertexColor = data.SubTexture->GetTexture()->IsCorrect() ? vertexColor : glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};
    }

    if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
      Flush();

    for (uint32_t i{0}; i < quadVertexCount; i++) {
      s_Data.QuadVertexBufferPtr->Pos = data.Transform * s_Data.QuadVertexPositions[i];
      s_Data.QuadVertexBufferPtr->Color = data.Color;
      s_Data.QuadVertexBufferPtr->TexIndex = (float)textureIndex;
      s_Data.QuadVertexBufferPtr->TilingFactor = data.Tiling;
      s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
      s_Data.QuadVertexBufferPtr->EntityID = data.EntityID;
      s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;
    s_Data.Stats.QuadCount++;
  }

  void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color) {
    DrawQuad(transform, (Ref < Texture2D > )
    nullptr, color);
  }

  void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref <Texture2D>& texture,
                            const glm::vec4& color, float tilingFactor) {
  }

  void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref <SubTexture2D>& subTexture, const glm::vec4& color,
                            float tilingFactor) {
    constexpr uint32_t quadVertexCount = 4;

    AGE_CORE_ASSERT(subTexture, "There is no sub-texture provided")

    const Ref <Texture2D> texture = subTexture->GetTexture();
    const glm::vec2* textureCoords = subTexture->TexCoords();
    const uint32_t textureIndex = FindTextureIndex(texture);

    glm::vec4 vertexColor{color};
    if (texture && !texture->IsCorrect()) {
      vertexColor = {1.0f, 1.0f, 1.0f, 1.0f};
    }

    if (s_Data.QuadIndexCount >= s_Data.MaxIndices)
      Flush();

    for (uint32_t i{0}; i < quadVertexCount; i++) {
      s_Data.QuadVertexBufferPtr->Pos = transform * s_Data.QuadVertexPositions[i];
      s_Data.QuadVertexBufferPtr->Color = color;
      s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
      s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
      s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
      s_Data.QuadVertexBufferPtr++;
    }

    s_Data.QuadIndexCount += 6;
    s_Data.Stats.QuadCount++;
  }

  void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color) {
    AGE_PROFILE_FUNCTION();

    DrawQuad(glm::vec3{pos, 0.0f}, size, color);
  }

  void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color) {
    AGE_PROFILE_FUNCTION();

    DrawQuad(pos, size, s_Data.UnitTexture, color);
  }

  void Renderer2D::DrawQuad(
      const glm::vec2& pos, const glm::vec2& size, const Ref <Texture2D>& texture,
      const float tilingFactor) {
    AGE_PROFILE_FUNCTION();

    DrawQuad(glm::vec3(pos, 0.0f), size, texture, tilingFactor);
  }

  void Renderer2D::DrawQuad(
      const glm::vec3& pos, const glm::vec2& size, const Ref <Texture2D>& texture,
      const float tilingFactor) {
    AGE_PROFILE_FUNCTION();

    DrawQuad(pos, size, texture, glm::vec4(1.0f), tilingFactor);
  }

  void Renderer2D::DrawQuad(
      const glm::vec2& pos, const glm::vec2& size, const Ref <Texture2D>& texture,
      const glm::vec4& color, float tilingFactor) {
    AGE_PROFILE_FUNCTION();

    DrawQuad(glm::vec3(pos, 0.0f), size, texture, color, tilingFactor);
  }

  void Renderer2D::DrawQuad(
      const glm::vec3& pos, const glm::vec2& size, const Ref <Texture2D>& texture,
      const glm::vec4& color, const float tilingFactor) {
    AGE_PROFILE_FUNCTION();

    const glm::mat4 transform =
        glm::translate(glm::mat4{1.0f}, pos) * glm::scale(glm::mat4{1.0f}, {size.x, size.y, 1.0f});

    DrawQuad(transform, texture, color, tilingFactor);
  }

  void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref <SubTexture2D>& subTexture,
                            float tilingFactor) {
    DrawQuad(glm::vec3{pos, 0.0f}, size, subTexture, glm::vec4{1.0f}, tilingFactor);
  }

  void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref <SubTexture2D>& subTexture,
                            float tilingFactor) {
    DrawQuad(pos, size, subTexture, glm::vec4{1.0f}, tilingFactor);
  }

  void Renderer2D::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref <SubTexture2D>& subTexture,
                            const glm::vec4& color, float tilingFactor) {
    DrawQuad(glm::vec3{pos, 0.0f}, size, subTexture, color, tilingFactor);
  }

  void Renderer2D::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref <SubTexture2D>& subTexture,
                            const glm::vec4& color, float tilingFactor) {
    const glm::mat4 transform =
        glm::translate(glm::mat4{1.0f}, pos) * glm::scale(glm::mat4{1.0f}, {size.x, size.y, 1.0f});

    DrawQuad(transform, subTexture, color, tilingFactor);
  }

  int Renderer2D::FindTextureIndex(const Ref <Texture2D>& texture) {
    int textureIndex = -1;

    if (texture != nullptr) {
      for (int i{0}; i < s_Data.TextureSlotIndex; i++) {
        if (*s_Data.TextureSlots[i].get() == *texture.get()) {
          textureIndex = i;
          break;
        }
      }

      if (textureIndex == -1) {
        textureIndex = (int)s_Data.TextureSlotIndex;
        s_Data.TextureSlots[textureIndex] = texture;
        s_Data.TextureSlotIndex++;
      }
    }
    return textureIndex;
  }

#pragma endregion

#pragma region DrawRotatedQuad

  // /////////////////////////////////////////////////////////////////////////////////////////////////
  //                                        DRAW ROTATED QUAD                                       //
  // /////////////////////////////////////////////////////////////////////////////////////////////////

  void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const float rotationDeg,
                                   const glm::vec4& color) {
    DrawRotatedQuad(glm::vec3{pos, 0.0f}, size, rotationDeg, s_Data.UnitTexture, color, 1.0f);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const float rotationDeg,
                                   const glm::vec4& color) {
    DrawRotatedQuad(pos, size, rotationDeg, s_Data.UnitTexture, color, 1.0f);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const float rotationDeg,
                                   const Ref <Texture2D>& texture, float tilingFactor) {
    DrawRotatedQuad(glm::vec3{pos, 0.0f}, size, rotationDeg, s_Data.UnitTexture, glm::vec4{1.0f}, 1.0f);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, const float rotationDeg,
                                   const Ref <Texture2D>& texture, float tilingFactor) {
    DrawRotatedQuad(pos, size, rotationDeg, s_Data.UnitTexture, glm::vec4{1.0f}, 1.0f);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, const float rotationDeg,
                                   const Ref <Texture2D>& texture, const glm::vec4& color, float tilingFactor) {
    DrawRotatedQuad(glm::vec3{pos, 0.0f}, size, rotationDeg, s_Data.UnitTexture, glm::vec4{1.0f}, tilingFactor);
  }

  void Renderer2D::DrawRotatedQuad(
      const glm::vec3& pos, const glm::vec2& size, const float rotationDeg, const Ref <Texture2D>& texture,
      const glm::vec4& color, float tilingFactor) {
    const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
                                * glm::rotate(glm::mat4(1.0f), glm::radians(rotationDeg), {0.0f, 0.0f, 1.0f})
                                * glm::scale(glm::mat4(1.0f), {size.x, size.y, 0.0f});

    DrawQuad(transform, texture, color, tilingFactor);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotationDeg,
                                   const Ref <SubTexture2D>& subTexture, float tilingFactor) {
    DrawRotatedQuad(glm::vec3{pos, 0.0f}, size, rotationDeg, subTexture, glm::vec4{1.0f}, tilingFactor);
  }

  void Renderer2D::DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotationDeg,
                                   const Ref <SubTexture2D>& subTexture, float tilingFactor) {
    DrawRotatedQuad(pos, size, rotationDeg, subTexture, glm::vec4{1.0f}, tilingFactor);
  }

  void Renderer2D::DrawRotatedQuad(
      const glm::vec2& pos, const glm::vec2& size, float rotationDeg, const Ref <SubTexture2D>& subTexture,
      const glm::vec4& color, float tilingFactor) {
    DrawRotatedQuad(glm::vec3{pos, 0.0f}, size, rotationDeg, subTexture, color, tilingFactor);
  }

  void Renderer2D::DrawRotatedQuad(
      const glm::vec3& pos, const glm::vec2& size, float rotationDeg, const Ref <SubTexture2D>& subTexture,
      const glm::vec4& color, float tilingFactor) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos)
                          * glm::rotate(glm::mat4(1.0f), glm::radians(rotationDeg), {0.0f, 0.0f, 1.0f})
                          * glm::scale(glm::mat4(1.0f), {size.x, size.y, 0.0f});

    DrawQuad(transform, subTexture, color, tilingFactor);
  }

#pragma endregion

#pragma region Statistics

  Renderer2D::Statistics Renderer2D::Stats() {
    return s_Data.Stats;
  }

  void Renderer2D::ResetStats() {
    memset(&s_Data.Stats, 0, sizeof(Renderer2D::Statistics));
  }

#pragma endregion

}// namespace AGE

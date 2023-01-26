//
// Created by alex on 18.10.22.
//

#ifndef AGE_RENDERER2D_H
#define AGE_RENDERER2D_H

#include <glm/glm.hpp>

#include "OrthographicCamera.h"
#include "Texture.h"

namespace AGE {

  /**
   * @brief A renderer class with a set of static methods to work with 2D graphics.
   * @attention Before use of the renderer Renderer2D::Init() must be called.
   */
  class Renderer2D {
  public:
    static void Init();
    static void ShutDown();

    static void BeginScene(const OrthographicCamera& camera);
    static void EndScene();

    static void StartBatch();
    static void NextBatch();
    static void Flush();

#pragma region DrawQuad

    static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor = 1.0f);
    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, float tillingFactor = 1.0f);
    static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color);

    /**
     * @brief Adds quad to current batch.
     * @param pos - world position.
     * @param size - size in meters.
     * @param texture - texture. If none, white 1x1 texture will be used.
     * @param color - tint color.
     * @param tillingFactor - tilling factor for the texture.
     */
    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& color, float tillingFactor = 1.0f);

#pragma endregion

#pragma region DrawRotatedQuad

    static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotationDeg, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotationDeg, const glm::vec4& color);
    static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotationDeg, const Ref<Texture2D>& texture, float tillingFactor = 1.0f);
    static void DrawRotatedQuad(const glm::vec3& pos, const glm::vec2& size, float rotationDeg, const Ref<Texture2D>& texture, float tillingFactor = 1.0f);
    static void DrawRotatedQuad(const glm::vec2& pos, const glm::vec2& size, float rotationDeg, const Ref<Texture2D>& texture, const glm::vec4& color);

    /**
     * @brief Adds a rotated quad to the current batch.
     * @param pos - world position.
     * @param size - size in meters.
     * @param rotationDeg  - rotation in degrees.
     * @param texture - texture. If none, will be replaced with 1x1 white texture.
     * @param color - tint color.
     * @param tillingFactor - tilling factor for the texture.
     */
    static void DrawRotatedQuad(
        const glm::vec3& pos, const glm::vec2& size, float rotationDeg, const Ref<Texture2D>& texture, const glm::vec4& color, float tillingFactor = 1.0f
    );

#pragma endregion

    struct Statistics {
      uint32_t DrawCalls = 0;
      uint32_t QuadCount = 0;

      [[nodiscard]] inline uint32_t TotalVertexCount() const { return QuadCount * 4; }
      [[nodiscard]] inline uint32_t TotalIndexCount() const { return QuadCount * 6; }
    };

    [[nodiscard]] static Statistics Stats();
    static void ResetStats();

  private:
    [[nodiscard]] static int FindTextureIndex(const Ref<Texture2D>& texture);
  };

} // AGE

#endif //AGE_RENDERER2D_H

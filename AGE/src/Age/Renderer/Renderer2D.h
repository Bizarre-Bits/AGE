//
// Created by alex on 18.10.22.
//

#ifndef AGE_RENDERER2D_H
#define AGE_RENDERER2D_H

#include <glm/glm.hpp>

#include "OrthographicCamera.h"
#include "Texture.h"

namespace AGE {

  class Renderer2D {
  public:
    static void Init();
    static void ShutDown();

    static void BeginScene(const OrthographicCamera& camera);
    static void EndScene();

    static void StartBatch();
    static void NextBatch();
    static void Flush();

    // Primitives
    static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color);
    static void
    DrawQuad(
        const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
        float tillingFactor
    );
    static void
    DrawQuad(
        const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
        float tillingFactor = 1.0f
    );
    static void DrawQuad(
        const glm::vec2& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
        const glm::vec4& color
    );
    static void DrawQuad(
        const glm::vec3& pos, const glm::vec2& size, const Ref<Texture2D>& texture,
        const glm::vec4& color, float tillingFactor = 1.0f
    );

    static void DrawRotatedQuad(
        const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture,
        const glm::vec4& color, float tillingFactor = 1.0f
    );

  private:
    static int FindTextureIndex(const Ref<Texture2D>& texture);
  };

} // AGE

#endif //AGE_RENDERER2D_H

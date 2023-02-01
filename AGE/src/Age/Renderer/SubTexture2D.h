//
// Created by alex on 1/31/23.
//

#ifndef AGE_SUBTEXTURE2D_H
#define AGE_SUBTEXTURE2D_H

#include "Age/Core/Core.h"
#include "Texture.h"
#include "glm/glm.hpp"

namespace AGE {

  class SubTexture2D {
  public:
    SubTexture2D(const Ref <Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

    [[nodiscard]] const glm::vec2* TexCoords() const { return m_TexCoords; }
    [[nodiscard]] const Ref <Texture2D>& GetTexture() const { return m_Texture; }

    static Ref <SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& offset, const glm::vec2& cellSizePx, const glm::vec2& spriteSize = {1.0f, 1.0f});

  private:
    Ref <Texture2D> m_Texture;
    glm::vec2       m_TexCoords[4]{};
  };

} // AGE

#endif //AGE_SUBTEXTURE2D_H

//
// Created by alex on 1/31/23.
//

#include "agepch.h"

#include "SubTexture2D.h"

namespace AGE {
  SubTexture2D::SubTexture2D(const Ref <Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) : m_Texture{texture} {
    m_TexCoords[0] = {min.x, max.y};
    m_TexCoords[1] = {max.x, max.y};
    m_TexCoords[2] = {max.x, min.y};
    m_TexCoords[3] = {min.x, min.y};
  }

  Ref <SubTexture2D> SubTexture2D::CreateFromCoords(const Ref <Texture2D>& texture, const glm::vec2& offset, const glm::vec2& cellSizePx, const glm::vec2& spriteSize) {
    const auto sheetWidth = (float)texture->Width(), sheetHeight = (float)texture->Height();

    const glm::vec2 min{
        (offset.x * cellSizePx.x) / sheetWidth, 1.0f - ((offset.y * cellSizePx.y * spriteSize.y) / sheetHeight) - (cellSizePx.y * spriteSize.y) / sheetHeight
    };
    const glm::vec2 max{
        min.x + (spriteSize.x * cellSizePx.x / sheetWidth), 1.0f - (offset.y * cellSizePx.y) / sheetHeight
    };

    return MakeRef<SubTexture2D>(texture, min, max);
  }
} // AGE

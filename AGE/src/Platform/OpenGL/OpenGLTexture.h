//
// Created by alex on 11.10.22.
//

#ifndef AGE_OPENGLTEXTURE_H
#define AGE_OPENGLTEXTURE_H

#include "Age/Renderer/Texture.h"

namespace AGE {
  class OpenGLTexture2D : public Texture2D {
  public:
    OpenGLTexture2D(const age_string_t& path);
    virtual ~OpenGLTexture2D() override;

    virtual inline uint32_t GetWidth() const override { return m_Width; }
    virtual inline uint32_t GetHeight() const override { return m_Height; }

    virtual void Bind(uint32_t slot = 0) const override;

  private:
    uint32_t     m_Height, m_Width;
    age_string_t m_Path;
    uint32_t     m_RendererID;
  };
} // AGE

#endif //AGE_OPENGLTEXTURE_H

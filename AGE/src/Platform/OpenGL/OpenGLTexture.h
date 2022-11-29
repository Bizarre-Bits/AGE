//
// Created by alex on 11.10.22.
//

#ifndef AGE_OPENGLTEXTURE_H
#define AGE_OPENGLTEXTURE_H

#include "Age/Renderer/Texture.h"
#include "OpenGLPlatform.h"

namespace AGE {
  class OpenGLTexture2D : public Texture2D {
  public:
    OpenGLTexture2D(const age_string_t& path);
    OpenGLTexture2D(const uint32_t width, uint32_t height);
    virtual ~OpenGLTexture2D() override;

    virtual void SetData(void* data, uint32_t size) override;

    virtual inline uint32_t GetWidth() const override { return m_Width; }
    virtual inline uint32_t GetHeight() const override { return m_Height; }
    virtual inline uint32_t ID() const override { return m_RendererID; }

    virtual void Bind(uint32_t slot = 0) const override;
    virtual void Unbind() const override;

    virtual bool IsCorrect() const override;
    virtual uint32_t Slot() const override;

    virtual inline bool operator==(const Texture2D& other) const override {
      return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
    }

  private:
    uint32_t     m_Height, m_Width;
    age_string_t m_Path;
    uint32_t     m_RendererID;
    bool         m_isCorrect;

    GLenum m_InternalFormat, m_DataFormat;

    mutable uint32_t m_Slot;
    mutable bool     m_isBound;
    void SetErrorTexture();
  };
} // AGE

#endif //AGE_OPENGLTEXTURE_H

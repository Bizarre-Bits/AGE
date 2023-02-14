//
// Created by alex on 11.10.22.
//

#include "agepch.h"

#include <stb_image.h>
#include <bitset>

#include "OpenGLMaster.h"
#include "OpenGLTexture.h"

namespace AGE {
  OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, uint32_t height) : m_Width(width),
                                                                            m_Height(height),
                                                                            m_InternalFormat(
                                                                                GL_RGBA8
                                                                            ),
                                                                            m_DataFormat(GL_RGBA),
                                                                            m_isCorrect{true} {
    AGE_PROFILE_FUNCTION();

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }

  OpenGLTexture2D::OpenGLTexture2D(const age_string_t& path) : m_Path(path), m_Slot{0},
                                                               m_isBound{false},
                                                               m_InternalFormat(0),
                                                               m_DataFormat(0),
                                                               m_isCorrect{true} {
    AGE_PROFILE_FUNCTION();

    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (!data) {
      AGE_CORE_WARN("Failed to load an image from {0}, loading an error texture", path);
      SetErrorTexture();
      return;
    }

    m_Width  = width;
    m_Height = height;

    if (channels == 4) {
      m_InternalFormat = GL_RGBA8;
      m_DataFormat     = GL_RGBA;
    } else if (channels == 3) {
      m_InternalFormat = GL_RGB8;
      m_DataFormat     = GL_RGB;
    }

    AGE_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Image format is not supported");

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(
        m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data
    );

    stbi_image_free(data);
  }

  void OpenGLTexture2D::SetData(void* data, uint32_t size) {
    AGE_PROFILE_FUNCTION();

    uint32_t bytesPerPixel{0};
    switch (m_DataFormat) {
      case GL_RGBA:
        bytesPerPixel = 4;
        break;
      case GL_RGB:
        bytesPerPixel = 3;
        break;
      default: {
        AGE_CORE_ASSERT(false, "Image format is not supported");
        break;
      }
    }

    AGE_CORE_ASSERT_FORMATTED(m_Width * m_Height * bytesPerPixel == size,
                              "Wrong buffer size. Must be: {0}, provided: {1}",
                              m_Width * m_Height * bytesPerPixel, size);

    glTextureSubImage2D(
        m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data
    );
  }

  OpenGLTexture2D::~OpenGLTexture2D() {
    AGE_PROFILE_FUNCTION();

    glDeleteTextures(1, &m_RendererID);
  }

  void OpenGLTexture2D::Bind(uint32_t slot) const {
    AGE_PROFILE_FUNCTION();

    glBindTextureUnit(slot, m_RendererID);
    m_Slot    = slot;
    m_isBound = true;
  }

  void OpenGLTexture2D::Unbind() const {
    AGE_PROFILE_FUNCTION();

    if (!m_isBound) {
      AGE_CORE_WARN("Trying to unbind a texture that is not bound!");
      return;
    }
    m_isBound = false;
    glBindTextureUnit(m_Slot, 0);
  }

  bool OpenGLTexture2D::IsCorrect() const {
    AGE_PROFILE_FUNCTION();

    return m_isCorrect;
  }

  uint32_t OpenGLTexture2D::Slot() const {
    AGE_PROFILE_FUNCTION();

    return m_Slot;
  }

  void OpenGLTexture2D::SetErrorTexture() {
    AGE_PROFILE_FUNCTION();

    m_Height         = m_Width = 4;
    m_DataFormat     = GL_RGBA;
    m_InternalFormat = GL_RGBA8;
    m_isCorrect      = false;

    uint32_t data[]{
        0xffff00ff,
        0xffff00ff,
        0xff000000,
        0xff000000,

        0xffff00ff,
        0xffff00ff,
        0xff000000,
        0xff000000,

        0xff000000,
        0xff000000,
        0xffff00ff,
        0xffff00ff,

        0xff000000,
        0xff000000,
        0xffff00ff,
        0xffff00ff,
    };

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    SetData(&data, sizeof(data));
  }

  Ref <OpenGLTexture2D> OpenGLTexture2D::ErrorTextureImpl() {
    static Ref<OpenGLTexture2D> errorTexture{MakeRef<OpenGLTexture2D>()};
    return errorTexture;
  }

  OpenGLTexture2D::OpenGLTexture2D() {
    SetErrorTexture();
  }
} // AGE

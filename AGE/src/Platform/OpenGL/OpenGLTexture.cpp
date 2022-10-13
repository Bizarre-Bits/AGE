//
// Created by alex on 11.10.22.
//

#include "agepch.h"

#include <stb_image.h>

#include "OpenGLPlatform.h"
#include "OpenGLTexture.h"
#include "Age/Debug/Assert.h"

namespace AGE {
  OpenGLTexture2D::OpenGLTexture2D(const age_string_t& path) : m_Path(path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    AGE_CORE_ASSERT(data, "Failed to load an image");
    m_Width  = width;
    m_Height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
  }

  OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &m_RendererID);
  }

  void OpenGLTexture2D::Bind(uint32_t slot) const {
    glBindTextureUnit(0, m_RendererID);
  }
} // AGE
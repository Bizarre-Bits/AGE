//
// Created by alex on 2/2/23.
//
#include "agepch.h"

#include "OpenGLPlatform.h"
#include "OpenGLFramebuffer.h"

namespace AGE {
  OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specs) : m_Specification(specs) {
    Invalidate();
  }

  OpenGLFramebuffer::~OpenGLFramebuffer() {
    glDeleteFramebuffers(1, &m_RenderID);
    glDeleteTextures(1, &m_ColorAttachment);
    glDeleteTextures(1, &m_DepthAttachment);
  }

  const FramebufferSpecification& OpenGLFramebuffer::Specification() const {
    return m_Specification;
  }

  void OpenGLFramebuffer::Invalidate() {
    if (m_RenderID) {
      glDeleteFramebuffers(1, &m_RenderID);
      glDeleteTextures(1, &m_ColorAttachment);
      glDeleteTextures(1, &m_DepthAttachment);
    }

    glCreateFramebuffers(1, &m_RenderID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, (int)m_Specification.Width, (int)m_Specification.Height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, (int)m_Specification.Width, (int)m_Specification.Height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

    AGE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGLFramebuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);
    glViewport(0, 0, m_Specification.Width, m_Specification.Height);
  }

  void OpenGLFramebuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
    if (!width || !height)
      return;

    if (width == m_Specification.Width && height == m_Specification.Height)
      return;

    m_Specification.Width  = width;
    m_Specification.Height = height;
    Invalidate();
  }
} // AGE

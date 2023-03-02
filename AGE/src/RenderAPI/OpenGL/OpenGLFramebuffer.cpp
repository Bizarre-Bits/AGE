//
// Created by alex on 2/2/23.
//
#include "agepch.h"

#include "OpenGLFramebuffer.h"
#include "OpenGLMaster.h"
#include "OpenGLFramebufferUtils.h"

namespace AGE {
  OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specs) : m_Specification(specs) {
    for (auto attachmentSpecs: m_Specification.Attachments.Attachments) {
      if (!FramebufferUtils::IsDepthFormat(attachmentSpecs.TextureFormat)) {
        m_ColorAttachmentSpecifications.emplace_back(attachmentSpecs);
      } else {
        m_DepthAttachmentSpecification = attachmentSpecs;
      }
    }
    Invalidate();
  }

  OpenGLFramebuffer::~OpenGLFramebuffer() {
    glDeleteFramebuffers(1, &m_RenderID);
    glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
    glDeleteTextures(1, &m_DepthAttachment);
  }

  const FramebufferSpecification& OpenGLFramebuffer::Specification() const {
    return m_Specification;
  }

  void OpenGLFramebuffer::Invalidate() {
    if (m_RenderID) {
      glDeleteFramebuffers(1, &m_RenderID);
      glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
      glDeleteTextures(1, &m_DepthAttachment);

      m_ColorAttachments.clear();
      m_DepthAttachment = 0;
    }

    glCreateFramebuffers(1, &m_RenderID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_RenderID);

    // Attachments
    bool isMultisample = m_Specification.Samples > 1;

    if (!m_ColorAttachmentSpecifications.empty()) {
      m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
      FramebufferUtils::CreateTextures(isMultisample, m_ColorAttachments.data(), m_ColorAttachments.size());

      for (size_t i{0}; i < m_ColorAttachments.size(); ++i) {
        FramebufferUtils::BindTexture(isMultisample, m_ColorAttachments[i]);
        GLenum internalFormat = FramebufferUtils::GlInternalTextureFormat(
            m_ColorAttachmentSpecifications[i].TextureFormat
        );

        FramebufferUtils::AttachColorTexture(
            m_ColorAttachments[i], i, m_Specification.Samples, internalFormat,
            m_Specification.Width, m_Specification.Height
        );
      }
    }

    if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {
      FramebufferUtils::CreateTextures(isMultisample, &m_DepthAttachment, 1);
      FramebufferUtils::BindTexture(isMultisample, m_DepthAttachment);
      GLenum framebufferTarget, internalFormat;
      switch (m_DepthAttachmentSpecification.TextureFormat) {
        case FramebufferTextureFormat::DEPTH24STENCIL8:
          internalFormat = GL_DEPTH24_STENCIL8;
          framebufferTarget = GL_DEPTH_STENCIL_ATTACHMENT;
          break;
        default:
          framebufferTarget = 0;
          internalFormat = 0;
          break;
      }
      FramebufferUtils::AttachDepthTexture(
          m_DepthAttachment, m_Specification.Samples, internalFormat, framebufferTarget,
          m_Specification.Width, m_Specification.Height
      );
    }

    if (m_ColorAttachments.size() >= 1) {
      AGE_CORE_ASSERT(m_ColorAttachments.size() <= 4);
      GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
      glDrawBuffers(m_ColorAttachments.size(), buffers);
    } else if (m_ColorAttachments.empty()) {
      glDrawBuffer(GL_NONE);
    }

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

    m_Specification.Width = width;
    m_Specification.Height = height;
    Invalidate();
  }

  int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
    AGE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixelData{0};
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
  }

  void OpenGLFramebuffer::ClearAttachment(uint32_t index, const void* value) {
    AGE_CORE_ASSERT(index < m_ColorAttachments.size());
    FramebufferTextureFormat textureFormat = m_ColorAttachmentSpecifications[index].TextureFormat;
    glClearTexImage(
        m_ColorAttachments[index],
        0,
        FramebufferUtils::GlTextureFormat(textureFormat),
        FramebufferUtils::GLDataType(textureFormat),
        value
    );
  }

} // AGE

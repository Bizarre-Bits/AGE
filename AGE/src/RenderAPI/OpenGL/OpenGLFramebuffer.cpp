//
// Created by alex on 2/2/23.
//
#include "agepch.h"

#include "OpenGLFramebuffer.h"
#include "OpenGLMaster.h"

namespace AGE {
  namespace LocalUtils {
    bool IsDepthFormat(FramebufferTextureFormat format) {
      using
      enum FramebufferTextureFormat;
      switch (format) {
        case DEPTH24STENCIL8:
          return true;
        default:
          return false;
      }
    }

    GLenum TextureTarget(bool multisample) {
      return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    void BindTexture(bool multisample, uint32_t id) {
      glBindTexture(TextureTarget(multisample), id);
    }

    void CreateTextures(bool multisample, uint32_t* outColorAttachments, uint32_t count) {
      glCreateTextures(TextureTarget(multisample), count, outColorAttachments);
    }

    void AttachColorTexture(
        uint32_t colorAttachmentID, uint32_t index, uint32_t samples, GLenum nativeFormat, uint32_t width,
        uint32_t height
    ) {
      if (nativeFormat == 0)
        return;

      bool isMultisample = samples > 1;
      GLenum target = TextureTarget(isMultisample);
      if (isMultisample) {
        glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (int)samples, nativeFormat, width, height, GL_FALSE);
      } else {
        glTexStorage2D(GL_TEXTURE_2D, 1, nativeFormat, width, height);
      }

      glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target, colorAttachmentID, 0);
    }

    void AttachDepthTexture(
        uint32_t depthAttachmentID, uint32_t samples, GLenum nativeFormat, GLenum framebufferTarget,
        uint32_t width, uint32_t height
    ) {
      if (nativeFormat == 0 || framebufferTarget == 0)
        return;

      bool isMultisample = samples > 1;
      GLenum target = TextureTarget(isMultisample);
      if (isMultisample) {
        glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, nativeFormat, width, height, GL_FALSE);
      } else {
        glTexStorage2D(GL_TEXTURE_2D, 1, nativeFormat, width, height);
      }

      glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      glFramebufferTexture2D(GL_FRAMEBUFFER, framebufferTarget, target, depthAttachmentID, 0);
    }
  }

  OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& specs) : m_Specification(specs) {
    for (auto attachmentSpecs: m_Specification.Attachments.Attachments) {
      if (!LocalUtils::IsDepthFormat(attachmentSpecs.TextureFormat)) {
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
      LocalUtils::CreateTextures(isMultisample, m_ColorAttachments.data(), m_ColorAttachments.size());

      for (size_t i{0}; i < m_ColorAttachments.size(); ++i) {
        LocalUtils::BindTexture(isMultisample, m_ColorAttachments[i]);
        GLenum nativeTextureFormat;
        switch (m_ColorAttachmentSpecifications[i].TextureFormat) {
          case FramebufferTextureFormat::RGBA8: {
            nativeTextureFormat = GL_RGBA8;
            break;
          }
          default:
            nativeTextureFormat = 0;
            break;
        }

        LocalUtils::AttachColorTexture(
            m_ColorAttachments[i], i, m_Specification.Samples, nativeTextureFormat,
            m_Specification.Width, m_Specification.Height
        );
      }
    }

    if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {
      LocalUtils::CreateTextures(isMultisample, &m_DepthAttachment, 1);
      LocalUtils::BindTexture(isMultisample, m_DepthAttachment);
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
      LocalUtils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, internalFormat, framebufferTarget,
                                     m_Specification.Width, m_Specification.Height);
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
} // AGE

//
// Created by alex on 2/27/23.
//

#include "OpenGLFramebufferUtils.h"

namespace AGE::FramebufferUtils {
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

  void AttachColorTexture(uint32_t colorAttachmentID, uint32_t index, uint32_t samples, GLenum internalFormat,
                          uint32_t width, uint32_t height) {
    if (internalFormat == 0)
      return;

    bool isMultisample = samples > 1;
    GLenum target = TextureTarget(isMultisample);
    if (isMultisample) {
      glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, (int)samples, internalFormat, width, height, GL_FALSE);
    } else {
      glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, width, height);
    }

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target, colorAttachmentID, 0);
  }

  void AttachDepthTexture(uint32_t depthAttachmentID, uint32_t samples, GLenum nativeFormat, GLenum framebufferTarget,
                          uint32_t width, uint32_t height) {

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

  GLenum GlInternalTextureFormat(FramebufferTextureFormat format) {
    switch (format) {
      case FramebufferTextureFormat::RGBA8:
        return GL_RGBA8;
      case FramebufferTextureFormat::RED_INT:
        return GL_R32I;
      case FramebufferTextureFormat::DEPTH24STENCIL8:
        return GL_DEPTH24_STENCIL8;
      default: {
        AGE_CORE_ASSERT(false);
        return 0;
      }
    }
  }

  GLenum GLDataType(FramebufferTextureFormat format) {
    switch (format) {
      case FramebufferTextureFormat::RGBA8:
      case FramebufferTextureFormat::DEPTH24STENCIL8:
        return GL_FLOAT;
      case FramebufferTextureFormat::RED_INT:
        return GL_INT;
      default: {
        AGE_CORE_ASSERT(false);
        return 0;
      }
    }
  }

  GLenum GlTextureFormat(FramebufferTextureFormat format) {
    switch (format) {
      case FramebufferTextureFormat::RGBA8:
        return GL_RGBA;
      case FramebufferTextureFormat::DEPTH24STENCIL8:
        return GL_DEPTH_STENCIL;
      case FramebufferTextureFormat::RED_INT:
        return GL_RED_INTEGER;
      default: {
        AGE_CORE_ASSERT(false);
        return 0;
      }
    }
    return 0;
  }
}
//
// Created by alex on 2/27/23.
//

#pragma once

#include "OpenGLMaster.h"
#include "Age/Renderer/Framebuffer.h"

namespace AGE::FramebufferUtils {
  bool IsDepthFormat(FramebufferTextureFormat format);

  inline GLenum TextureTarget(bool multisample) {
    return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
  }

  inline void BindTexture(bool multisample, uint32_t id) {
    glBindTexture(TextureTarget(multisample), id);
  }

  inline void CreateTextures(bool multisample, uint32_t* outColorAttachments, uint32_t count) {
    glCreateTextures(TextureTarget(multisample), count, outColorAttachments);
  }

  void AttachColorTexture(
      uint32_t colorAttachmentID, uint32_t index, uint32_t samples, GLenum internalFormat, uint32_t width,
      uint32_t height
  );

  void AttachDepthTexture(
      uint32_t depthAttachmentID, uint32_t samples, GLenum nativeFormat, GLenum framebufferTarget,
      uint32_t width, uint32_t height
  );

  GLenum GlInternalTextureFormat(FramebufferTextureFormat format);
  GLenum GlTextureFormat(FramebufferTextureFormat format);
  GLenum GLDataType(FramebufferTextureFormat format);
}

//
// Created by alex on 2/2/23.
//

#pragma once

#include "Age/Renderer/Framebuffer.h"

namespace AGE {

  class OpenGLFramebuffer : public Framebuffer {
  public:
    explicit OpenGLFramebuffer(const FramebufferSpecification& specs);
    virtual ~OpenGLFramebuffer() override;

    [[nodiscard]] const FramebufferSpecification& Specification() const override;

    [[nodiscard]] inline uint32_t ColorAttachmentID(uint32_t index) const override {
      AGE_CORE_ASSERT(index < m_ColorAttachments.size());
      return m_ColorAttachments[index];
    }

    virtual void Resize(uint32_t width, uint32_t height) override;

    virtual void Bind() override;
    virtual void Unbind() override;

    void Invalidate();
  private:
    FramebufferSpecification m_Specification;
    uint32_t m_RenderID{0};
    std::vector<uint32_t> m_ColorAttachments;
    uint32_t m_DepthAttachment;

    std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
    FramebufferTextureSpecification m_DepthAttachmentSpecification;
  };

} // AGE

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
    [[nodiscard]] inline uint32_t ColorAttachmentID() const override { return m_ColorAttachment; }

    virtual void Resize(uint32_t width, uint32_t height) override;

    virtual void Bind() override;
    virtual void Unbind() override;

    void Invalidate();
  private:
    FramebufferSpecification m_Specification;
    uint32_t                 m_RenderID{0};
    uint32_t                 m_ColorAttachment{0};
    uint32_t                 m_DepthAttachment{0};
  };

} // AGE

//
// Created by alex on 2/2/23.
//

#ifndef AGE_OPENGLFRAMEBUFFER_H
#define AGE_OPENGLFRAMEBUFFER_H

#include "Age/Renderer/Framebuffer.h"

namespace AGE {

  class OpenGLFramebuffer : public Framebuffer {
  public:
    explicit OpenGLFramebuffer(const FramebufferSpecification& specs);
    virtual ~OpenGLFramebuffer() override {}

    [[nodiscard]] const FramebufferSpecification& Specification() const override;
    [[nodiscard]] inline uint32_t ColorAttachmentID() const override { return m_ColorAttachment; }

    virtual void Bind() override;
    virtual void Unbind() override;

    void Invalidate();
  private:
    FramebufferSpecification m_Specification;
    uint32_t                 m_RenderID;
    uint32_t                 m_ColorAttachment;
    uint32_t                 m_DepthAttachment;
  };

} // AGE

#endif //AGE_OPENGLFRAMEBUFFER_H

//
// Created by alex on 2/2/23.
//

#ifndef AGE_FRAMEBUFFER_H
#define AGE_FRAMEBUFFER_H

#include "Age/Core/Core.h"

namespace AGE {
  struct FramebufferSpecification {
    uint32_t Width, Height;

    uint32_t Samples         = 1;
    bool     swapChainTarget = false;
  };

  class Framebuffer {
  public:
    virtual ~Framebuffer() = default;

    virtual const FramebufferSpecification& Specification() const = 0;
    virtual uint32_t ColorAttachmentID() const = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    static Ref <Framebuffer> Create(const FramebufferSpecification& spec);
  };

} // AGE

#endif //AGE_FRAMEBUFFER_H

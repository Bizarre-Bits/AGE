//
// Created by alex on 2/2/23.
//

#pragma once

#include "Age/Core/Core.h"

namespace AGE {
  enum class FramebufferTextureFormat {
    None = 0,

    // Color
    RGBA8,

    // Depth/stencil
    DEPTH24STENCIL8,

    // Defaults
    Depth = DEPTH24STENCIL8,
    Color = RGBA8
  };

  struct FramebufferTextureSpecification {
    FramebufferTextureSpecification() = default;

    FramebufferTextureSpecification(FramebufferTextureFormat format) : TextureFormat(format) {}

    FramebufferTextureFormat TextureFormat;
    // TODO: filtering/wrap
  };

  struct FramebufferAttachmentSpecification {
    FramebufferAttachmentSpecification() = default;

    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> specifications)
        : Attachments{specifications} {};

    std::vector<FramebufferTextureSpecification> Attachments;
  };

  struct FramebufferSpecification {
    uint32_t Width, Height;

    uint32_t Samples = 1;
    FramebufferAttachmentSpecification Attachments;
    bool swapChainTarget = false;
  };

  class Framebuffer {
  public:
    virtual ~Framebuffer() = default;

    virtual const FramebufferSpecification& Specification() const = 0;
    virtual uint32_t ColorAttachmentID(uint32_t index = 0) const = 0;

    virtual void Resize(uint32_t width, uint32_t height) = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
  };

} // AGE

//
// Created by alex on 2/2/23.
//

#include "agepch.h"

#include "Framebuffer.h"
#include "Renderer.h"

#if defined(AGE_INCLUDE_OPENGL)
  #include "Platform/OpenGL/OpenGLFramebuffer.h"
#endif

namespace AGE {
  Ref <Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec) {
    AGE_PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return MakeRef<OpenGLFramebuffer>(spec);
#else
        AGE_CORE_ASSER(false, "OpenGL is not available, because it is not included into the current compilation")
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a Frame Buffer, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }
} // AGE

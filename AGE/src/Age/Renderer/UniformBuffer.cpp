//
// Created by alex on 3/2/23.
//
#include "agepch.h"

#include "UniformBuffer.h"

#include "Renderer.h"
#include "RenderAPI.h"

#if defined(AGE_INCLUDE_OPENGL)
  #include "RenderAPI/OpenGL/OpenGLUniformBuffer.h"
#endif
#if defined(AGE_INCLUDE_VULKAN)
  #include "RenderAPI/Vulkan/VulkanUniformBuffer.h"
#endif

namespace AGE {
  Ref <UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
    AGE_PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
#if defined(AGE_INCLUDE_OPENGL)
        return MakeRef<OpenGLUniformBuffer>(size, binding);
#else
        AGE_CORE_ASSER(false, "OpenGL is not available, because it is not included into the current compilation")
#endif
      case RenderAPI::API::Vulkan:
#if defined(AGE_INCLUDE_VULKAN)
        return MakeRef<VulkanUniformBuffer>(size, binding);
#else
        AGE_CORE_ASSER(false, "Vulkan is not available, because it is not included into the current compilation")
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a Uniform Buffer, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }
} // AGE
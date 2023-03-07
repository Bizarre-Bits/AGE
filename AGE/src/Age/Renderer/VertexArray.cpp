//
// Created by alex on 09.09.22.
//

#include "agepch.h"

#include "VertexArray.h"
#include "Renderer.h"

#if defined(AGE_INCLUDE_OPENGL)
  #include "RenderAPI/OpenGL/OpenGLVertexArray.h"
#endif
#if defined(AGE_INCLUDE_VULKAN)
  #include "RenderAPI/Vulkan/VulkanVertexArray.h"
#endif

namespace AGE {
  Ref <VertexArray> VertexArray::Create() {
    AGE_PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
#if defined(AGE_INCLUDE_OPENGL)
        return MakeRef<OpenGLVertexArray>();
#else
        AGE_CORE_ASSER(false, "OpenGL is not available, because it is not included into the current compilation")
#endif

      case RenderAPI::API::Vulkan:
#if defined(AGE_INCLUDE_VULKAN)
        return MakeRef<VulkanVertexArray>();
#else
        AGE_CORE_ASSER(false, "Vulkan is not available, because it is not included into the current compilation")
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a Vertex Array, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }
} // AGE

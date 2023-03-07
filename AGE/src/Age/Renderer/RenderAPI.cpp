//
// Created by alex on 04.10.22.
//

#include "agepch.h"

#include "RenderAPI.h"

#if defined(AGE_INCLUDE_OPENGL)
  #include "RenderAPI/OpenGL/OpenGLRenderAPI.h"
#endif
#if defined(AGE_INCLUDE_VULKAN)
  #include "RenderAPI/Vulkan/VulkanRenderAPI.h"
#endif

namespace AGE {
  RenderAPI::API RenderAPI::s_API{API::Vulkan};

  Scope<RenderAPI> RenderAPI::Create() {
    AGE_PROFILE_FUNCTION();

    switch (s_API) {
      case API::None: AGE_CORE_ASSERT(false, "API::None is not supported yet.");
      case API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return CreateScope<OpenGLRenderAPI>();
#else
        AGE_CORE_ASSERT(false, "OpenGL is not included into the current compilation");
#endif
      case API::Vulkan:
#if defined(AGE_INCLUDE_VULKAN)
        return CreateScope<VulkanRenderAPI>();
#else
        AGE_CORE_ASSERT(false, "Vulkan is not included into the current compilaction");
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a RenderAPI object, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }
} // AGE

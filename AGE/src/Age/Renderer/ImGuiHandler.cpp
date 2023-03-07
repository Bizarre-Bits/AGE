//
// Created by alex on 2/5/23.
//
#include "agepch.h"

#include "ImGuiHandler.h"

#if defined(AGE_INCLUDE_OPENGL)
  #include "RenderAPI/OpenGL/OpenGLImGuiHandler.h"
#endif
#if defined(AGE_INCLUDE_VULKAN)
  #include "RenderAPI/Vulkan/VulkanRenderAPI.h"
#endif

#include "Renderer.h"
#include "RenderAPI.h"
#include "RenderAPI/OpenGL/OpenGLRenderAPI.h"
#include "RenderAPI/Vulkan/VulkanImGuiHandler.h"

namespace AGE {
  Ref<ImGuiHandler_PlatformImpl> ImGuiHandler::s_ImGuiPlatformAPI = ImGuiHandler::CreatePlatformAPI();

  Ref <ImGuiHandler_PlatformImpl> ImGuiHandler::CreatePlatformAPI() {
    AGE_PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return MakeRef<OpenGLImGuiHandler>();
#else
        AGE_CORE_ASSERT(false, "OpenGL is not available, because it is not included into the current compilation")
#endif
case RenderAPI::API::Vulkan:
#ifdef AGE_INCLUDE_VULKAN
        return MakeRef<VulkanImGuiHandler>();
#else
        AGE_CORE_ASSERT(false, "Vulkan is not available, because it is not included into the current compilation")
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a platform specific ImGui handler, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }
}

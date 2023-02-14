//
// Created by alex on 2/5/23.
//
#include "agepch.h"

#include "ImGuiHandler.h"

#if defined(AGE_INCLUDE_OPENGL)
  #include "RenderAPI/OpenGL/OpenGLImGuiHandler.h"
#endif

#include "Renderer.h"
#include "RenderAPI.h"

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
        AGE_CORE_ASSER(false, "OpenGL is not available, because it is not included into the current compilation")
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a platform specific ImGui handler, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }
}

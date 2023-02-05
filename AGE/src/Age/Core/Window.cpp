//
// Created by alex on 2/5/23.
//
#include "agepch.h"

#include "Window.h"

#if defined(AGE_INCLUDE_OPENGL)
  #include "Platform/OpenGL/OpenGLWindow.h"
#endif

#include "Age/Renderer/Renderer.h"
#include "Age/Renderer/RenderAPI.h"

namespace AGE {
  Scope<Window> Window::Create(const AGE::WindowProps& props) {
    AGE_PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return CreateScope<OpenGLWindow>(props);
#else
        AGE_CORE_ASSER(false, "OpenGL is not available, because it is not included into the current compilation")
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a platform specific Window, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }
}

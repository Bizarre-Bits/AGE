//
// Created by alex on 04.10.22.
//

#include "agepch.h"

#include "Age/Debug/Assert.h"
#include "RenderAPI.h"

#ifdef AGE_INCLUDE_OPENGL
  #include "Platform/OpenGL/OpenGLRenderAPI.h"
#endif

namespace AGE {
  RenderAPI::API RenderAPI::s_API{API::OpenGL};

  RenderAPI* RenderAPI::Create() {
    switch (s_API) {
      case API::None: AGE_CORE_ASSERT(false, "API::None is not supported yet.");
      case API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return new OpenGLRenderAPI;
#else
        AGE_CORE_ASSERT(false, "OpenGL is not included into the current compilation");
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a RenderAPI object, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }
} // AGE
//
// Created by alex on 09.09.22.
//

#include "agepch.h"

#include "Age/Debug/Assert.h"

#include "VertexArray.h"
#include "Renderer.h"

#ifdef AGE_INCLUDE_OPENGL
  #include "Platform/OpenGL/OpenGLVertexArray.h"
#endif

namespace AGE {
  Ref<VertexArray> VertexArray::Create() {
    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return std::make_shared<OpenGLVertexArray>();
#else
        AGE_CORE_ASSER(false, "OpenGL is not available, because it is not included into the current compilation")
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a Vertex Array, as there is no RendererAPI selected");
        throw;
      }
    }
  }
} // AGE
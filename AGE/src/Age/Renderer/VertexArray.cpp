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
  VertexArray* VertexArray::Create() {
    switch (Renderer::API()) {
      case RenderAPI::None: AGE_CORE_ASSERT(false, "RenderAPI::None is not supported");
      case RenderAPI::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return new OpenGLVertexArray();
#else
        AGE_CORE_ASSER(false, "RenderAPI::OpenGL is not available, because it is not included into the current compilation")
#endif
      default: {
        AGE_CORE_ASSERT(false, "Could not create a Vertex Array, as there is no RenderAPI selected");
        throw;
      }
    }
  }
} // AGE
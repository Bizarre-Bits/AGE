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
#ifdef AGE_INCLUDE_OPENGL
      case RenderAPI::OpenGL: return new OpenGLVertexArray();
#endif
    }
  }
} // AGE
//
// Created by alex on 09.09.22.
//

#include "agepch.h"

#include "Age/Debug/Assert.h"

#include "VertexArray.h"
#include "Renderer.h"

#ifdef AGE_OPENGL_AVAILABLE
  #include "Platform/OpenGL/OpenGLVertexArray.h"
#endif

namespace AGE {
  VertexArray* VertexArray::Create() {
    switch (Renderer::API()) {
      case RenderAPI::None: AGE_CORE_ASSERT(false, "RenderAPI::None is not supported");
      case RenderAPI::OpenGL: return new OpenGLVertexArray();
    }
  }
} // AGE
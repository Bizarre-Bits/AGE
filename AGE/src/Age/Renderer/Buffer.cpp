//
// Created by alex on 09.09.22.
//
#include "agepch.h"

#include "Age/Debug/Assert.h"

#include "Age/Renderer/Buffer.h"

#ifdef AGE_OPENGL_AVAILABLE
  #include "Platform/OpenGL/OpenGLBuffer.h"
#endif

namespace AGE {
  VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
    switch (Renderer::API()) {
      case RenderAPI::None: AGE_CORE_ASSERT(false, "RenderAPI::None is not supported");
      case RenderAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
    }
  }

  IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size) {
    switch (Renderer::API()) {
      case RenderAPI::None: AGE_CORE_ASSERT(false, "RenderAPI::None is not supported");
      case RenderAPI::OpenGL: return new OpenGLIndexBuffer(indices, size);
    }
  }
}
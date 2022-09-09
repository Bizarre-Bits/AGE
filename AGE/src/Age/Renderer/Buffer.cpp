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
  uint32_t BufferElement::ComponentCount() const {
    auto count = uint32_t((int)Type & 0b11111);
    AGE_CORE_ASSERT(count != 0, "Unknown ShaderDataType");
    return count;
  }

  BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
      : m_Elements{elements}, m_Stride{0} {
    calculateOffsetsAndStride();
  }

  void BufferLayout::calculateOffsetsAndStride() {
    uint32_t offset{0};
    m_Stride = 0;
    for (auto& el: m_Elements) {
      el.Offset = offset;
      offset += el.Size;
      m_Stride += el.Size;
    }
  }

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

  uint32_t shader_data_size(ShaderDataType type) {
    uint8_t dataType       = (uint8_t)type & 0b111'00000;
    uint8_t componentCount = (uint8_t)type & 0b000'11111;

    switch (dataType) {
      case 0: return sizeof(float)*componentCount;
      case 1: return sizeof(int)*componentCount;
      case 2: return sizeof(bool)*componentCount;
      default: break;
    }

    AGE_CORE_ASSERT(false, "Unknown ShaderDataType");
    return 0;
  }

  ShaderDataType base_shader_data_type(ShaderDataType type) {
    return (ShaderDataType)(((uint8_t)type & 0b111'00000) | 1);
  }
}
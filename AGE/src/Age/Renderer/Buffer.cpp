//
// Created by alex on 09.09.22.
//
#include "agepch.h"

#include "Age/Debug/Assert.h"
#include "Age/Renderer/Buffer.h"
#include "Renderer.h"
#include "RenderAPI.h"

#ifdef AGE_INCLUDE_OPENGL
  #include "Platform/OpenGL/OpenGLBuffer.h"
#endif

namespace AGE {
  uint32_t BufferElement::ComponentCount() const {
    AGE_PROFILE_FUNCTION();

    auto count = uint32_t((int)Type & 0b11111);
    AGE_CORE_ASSERT(count != 0, "Unknown ShaderDataType");
    return count;
  }

  BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
      : m_Elements{elements}, m_Stride{0} {
    calculateOffsetsAndStride();
  }

  void BufferLayout::calculateOffsetsAndStride() {
    AGE_PROFILE_FUNCTION();

    uint32_t offset{0};
    m_Stride = 0;
    for (auto& el: m_Elements) {
      el.Offset = offset;
      offset += el.Size;
      m_Stride += el.Size;
    }
  }

  Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t count) {
    AGE_PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
        //Sorry for this mess. TODO: should figure out a way to make it more readable.

        // OpenGL VertexBuffer
#ifdef AGE_INCLUDE_OPENGL
        return std::make_shared<OpenGLVertexBuffer>(vertices, count);
#else
        AGE_CORE_ASSERT(false, "RendererAPI::OpenGL is not available because OpenGL is not included in this compilation")
#endif

        // Handle unconfigured RendererAPI
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a vertex buffer, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }


  Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
    AGE_PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
        //Sorry for this mess. TODO: should figure out a way to make it more readable.

        // OpenGL VertexBuffer
#ifdef AGE_INCLUDE_OPENGL
        return std::make_shared<OpenGLVertexBuffer>(size);
#else
        AGE_CORE_ASSERT(false, "RendererAPI::OpenGL is not available because OpenGL is not included in this compilation")
#endif

        // Handle unconfigured RendererAPI
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a vertex buffer, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }

  Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
    AGE_PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return std::make_shared<OpenGLIndexBuffer>(indices, count);
#else
        AGE_CORE_ASSERT(false, "RendererAPI::OpenGL is not available because OpenGL is not included in this compilation");
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create an Index Buffer, as there is no RendererAPI selected")
      }
    }

    return nullptr;
  }

  uint32_t shader_data_size(ShaderDataType type) {
    AGE_PROFILE_FUNCTION();

    uint8_t dataType       = (uint8_t)type & 0b111'00000;
    uint8_t componentCount = (uint8_t)type & 0b000'11111;

    switch (dataType) {
      case 0:
        return sizeof(float) * componentCount;
      case 1:
        return sizeof(int) * componentCount;
      case 2:
        return sizeof(bool) * componentCount;
      default:
        break;
    }

    AGE_CORE_ASSERT(false, "Unknown ShaderDataType");
    return 0;
  }

  ShaderDataType base_shader_data_type(ShaderDataType type) {
    AGE_PROFILE_FUNCTION();

    return (ShaderDataType)(((uint8_t)type & 0b111'00000) | 1);
  }
}

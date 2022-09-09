//
// Created by alex on 09.09.22.
//

#include "agepch.h"
#include "Platform/OpenGL/OpenGLPlatform.h"

#include "OpenGLBuffer.h"
#include "Age/Debug/Assert.h"

namespace AGE {

////////////////////////////////////////////////////////////////////////////////////
/// Vertex buffer //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

  OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t count)
      : m_Count{count} {
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
  }

  void OpenGLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  }

  void OpenGLVertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  uint32_t OpenGLVertexBuffer::Count() const {
    return m_Count;
  }

////////////////////////////////////////////////////////////////////////////////////
/// Index buffer ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

  OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
      : m_Count{count} {
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
  }

  OpenGLIndexBuffer::~OpenGLIndexBuffer() {

  }

  void OpenGLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
  }

  void OpenGLIndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  uint32_t OpenGLIndexBuffer::Count() const {
    return m_Count;
  }


////////////////////////////////////////////////////////////////////////////////////
/// Free Functions /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

  GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    ShaderDataType baseType = base_shader_data_type(type);
    switch(baseType) {
      case ShaderDataType::Float: return GL_FLOAT;
      case ShaderDataType::Int: return GL_INT;
      case ShaderDataType::Bool: return GL_BOOL;
      default: break;
    }

    AGE_CORE_ASSERT(false, "Unknown ShaderDataType");
    return 0;
  }

} // AGE
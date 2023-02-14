//
// Created by alex on 09.09.22.
//

#include "agepch.h"
#include "RenderAPI/OpenGL/OpenGLMaster.h"

#include "OpenGLBuffer.h"

namespace AGE {

/*
 * ===============================================================
 * ||                  VertexBuffer                             ||
 * ===============================================================
 */

// It initializes m_Count with 0,
// because count property won't be needed for buffers generated this way, I guess
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size): m_Count{0} {
    AGE_PROFILE_FUNCTION();

    glGenBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
  }

  OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t count)
      : m_Count{count} {
    AGE_PROFILE_FUNCTION();

    glGenBuffers(1, &m_RenderID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, vertices, GL_STATIC_DRAW);
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    AGE_PROFILE_FUNCTION();

    glDeleteBuffers(1, &m_RenderID);
  }

  void OpenGLVertexBuffer::Bind() const {
    AGE_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
  }

  void OpenGLVertexBuffer::Unbind() const {
    AGE_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  uint32_t OpenGLVertexBuffer::Count() const {
    AGE_PROFILE_FUNCTION();

    return m_Count;
  }

  void OpenGLVertexBuffer::SetData(void* data, uint32_t size) {
    AGE_PROFILE_FUNCTION();

    glBindBuffer(GL_ARRAY_BUFFER, m_RenderID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
  }

/* ===============================================================
 * ||                   IndexBuffer                             ||
 * ===============================================================
 */

  OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
      : m_Count{count} {
    AGE_PROFILE_FUNCTION();

    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
  }

  OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
  }

  void OpenGLIndexBuffer::Bind() const {
    AGE_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
  }

  void OpenGLIndexBuffer::Unbind() const {
    AGE_PROFILE_FUNCTION();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  uint32_t OpenGLIndexBuffer::Count() const {
    AGE_PROFILE_FUNCTION();

    return m_Count;
  }


/* ===============================================================
 * ||                Free Functions                             ||
 * ===============================================================
 */

  GLenum shader_data_to_glenum(ShaderDataType type) {
    AGE_PROFILE_FUNCTION();

    ShaderDataType baseType = base_shader_data_type(type);
    switch (baseType) {
      case ShaderDataType::Float:
        return GL_FLOAT;
      case ShaderDataType::Int:
        return GL_INT;
      case ShaderDataType::Bool:
        return GL_BOOL;
      default:
        break;
    }

    AGE_CORE_ASSERT(false, "Unknown ShaderDataType");
    return 0;
  }

} // AGE

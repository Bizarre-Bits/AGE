//
// Created by alex on 09.09.22.
//

#include "agepch.h"

#include "OpenGLMaster.h"
#include "OpenGLVertexArray.h"

namespace AGE {
  OpenGLVertexArray::OpenGLVertexArray() {
    AGE_PROFILE_FUNCTION();

    glGenVertexArrays(1, &m_RenderID);
  }

  OpenGLVertexArray::~OpenGLVertexArray() {
    AGE_PROFILE_FUNCTION();

    glDeleteVertexArrays(1, &m_RenderID);
  }

  void OpenGLVertexArray::Bind() const {
    AGE_PROFILE_FUNCTION();

    glBindVertexArray(m_RenderID);
  }

  void OpenGLVertexArray::Unbind() const {
    AGE_PROFILE_FUNCTION();

    glBindVertexArray(0);
  }

  void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) {
    AGE_PROFILE_FUNCTION();

    AGE_CORE_ASSERT(!vertexBuffer->Layout().Elements().empty(), "Vertex buffer has no layout");

    glBindVertexArray(m_RenderID);
    vertexBuffer->Bind();

    uint32_t index{0};
    for (const BufferElement& element: vertexBuffer->Layout()) {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(
          index,
          element.ComponentCount(),
          shader_data_to_glenum(element.Type),
          element.Normalized ? GL_TRUE : GL_FALSE,
          vertexBuffer->Layout().Stride(),
          (void*)element.Offset
      );
      index++;
    }

    m_VertexBuffers.push_back(vertexBuffer);

    glBindVertexArray(0);
    vertexBuffer->Unbind();
  }

  void OpenGLVertexArray::SetIndexBuffer(Ref<AGE::IndexBuffer> indexBuffer) {
    AGE_PROFILE_FUNCTION();

    glBindVertexArray(m_RenderID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
    glBindVertexArray(0);
  }

  std::vector<Ref<VertexBuffer>> OpenGLVertexArray::VertexBuffers() const {
    AGE_PROFILE_FUNCTION();

    return m_VertexBuffers;
  }

  Ref<IndexBuffer> OpenGLVertexArray::IndexBuffer() const {
    AGE_PROFILE_FUNCTION();

    return m_IndexBuffer;
  }
} // AGE

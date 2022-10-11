//
// Created by alex on 09.09.22.
//

#include "agepch.h"

#include "OpenGLPlatform.h"
#include "Age/Debug/Assert.h"

#include "OpenGLVertexArray.h"

namespace AGE {
  OpenGLVertexArray::OpenGLVertexArray() {
    glGenVertexArrays(1, &m_RenderID);
  }

  OpenGLVertexArray::~OpenGLVertexArray() {
    glDeleteVertexArrays(1, &m_RenderID);
  }

  void OpenGLVertexArray::Bind() const {
    glBindVertexArray(m_RenderID);
  }

  void OpenGLVertexArray::Unbind() const {
    glBindVertexArray(0);
  }

  void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) {
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
    glBindVertexArray(m_RenderID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
    glBindVertexArray(0);
  }

  std::vector<Ref<VertexBuffer>> OpenGLVertexArray::VertexBuffers() const {
    return m_VertexBuffers;
  }

  Ref<IndexBuffer> OpenGLVertexArray::IndexBuffer() const {
    return m_IndexBuffer;
  }
} // AGE
//
// Created by alex on 3/2/23.
//
#include "agepch.h"

#include "OpenGLUniformBuffer.h"
#include "OpenGLMaster.h"

namespace AGE {
  OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
    glCreateBuffers(1, &m_RendererID);
    glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
  }

  OpenGLUniformBuffer::~OpenGLUniformBuffer() {
    glDeleteBuffers(1, &m_RendererID);
  }

  void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset) {
    glNamedBufferSubData(m_RendererID, offset, size, data);
  }
} // AGE
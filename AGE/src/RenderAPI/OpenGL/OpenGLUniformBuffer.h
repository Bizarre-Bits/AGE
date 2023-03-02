//
// Created by alex on 3/2/23.
//

#pragma once

#include "Age/Renderer/UniformBuffer.h"

namespace AGE {

  class OpenGLUniformBuffer : public UniformBuffer {
  public:
    OpenGLUniformBuffer(uint32_t size, uint32_t binding);
    ~OpenGLUniformBuffer() override;

    void SetData(const void* data, uint32_t size, uint32_t offset) override;
  private:
    uint32_t m_RendererID = 0;
  };

} // AGE

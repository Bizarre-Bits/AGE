//
// Created by alex on 3/7/23.
//

#pragma once

#include "Age/Renderer/UniformBuffer.h"

namespace AGE {

  class VulkanUniformBuffer : public UniformBuffer {
  public:
    VulkanUniformBuffer(uint32_t size, uint32_t binding);
    ~VulkanUniformBuffer() override;
    void SetData(const void* data, uint32_t size, uint32_t offset) override;

  };

} // AGE

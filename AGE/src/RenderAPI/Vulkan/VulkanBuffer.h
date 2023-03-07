//
// Created by alex on 3/7/23.
//

#pragma once

#include "Age/Renderer/Buffer.h"

namespace AGE {

  class VulkanVertexBuffer : public VertexBuffer {
  public:
    VulkanVertexBuffer(float* vertices, uint32_t count);
    VulkanVertexBuffer(uint32_t size);
    ~VulkanVertexBuffer() override;

    void SetLayout(const BufferLayout& layout) override;
    const BufferLayout& Layout() const override;

    void SetData(void* data, uint32_t size) override;

    void Bind() const override;
    void Unbind() const override;

    uint32_t Count() const override;

  private:
    BufferLayout m_Layout;
  };

  class VulkanIndexBuffer : public IndexBuffer {
  public:
    VulkanIndexBuffer(uint32_t* indices, uint32_t count);
    ~VulkanIndexBuffer() override;
    void Bind() const override;
    void Unbind() const override;
    uint32_t Count() const override;

  };
} // AGE

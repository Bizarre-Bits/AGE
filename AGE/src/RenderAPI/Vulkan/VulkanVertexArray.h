//
// Created by alex on 3/7/23.
//

#pragma once

#include "Age/Renderer/VertexArray.h"

namespace AGE {

  class VulkanVertexArray : public VertexArray {
  public:
    ~VulkanVertexArray() override;
    void Bind() const override;
    void Unbind() const override;
    void AddVertexBuffer(Ref <VertexBuffer> vertexBuffer) override;
    void SetIndexBuffer(Ref <AGE::IndexBuffer> indexBuffer) override;
    std::vector<Ref < VertexBuffer>> VertexBuffers()
    const override;
    Ref <AGE::IndexBuffer> IndexBuffer() const override;
  };

} // AGE

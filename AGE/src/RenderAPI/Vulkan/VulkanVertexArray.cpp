//
// Created by alex on 3/7/23.
//


#include "agepch.h"

#include "VulkanVertexArray.h"

namespace AGE {
  VulkanVertexArray::~VulkanVertexArray() {

  }

  void VulkanVertexArray::Bind() const {

  }

  void VulkanVertexArray::Unbind() const {

  }

  void VulkanVertexArray::AddVertexBuffer(Ref <VertexBuffer> vertexBuffer) {

  }

  void VulkanVertexArray::SetIndexBuffer(Ref <AGE::IndexBuffer> indexBuffer) {

  }

  std::vector<Ref < VertexBuffer>>

  VulkanVertexArray::VertexBuffers() const {
    return std::vector<Ref < VertexBuffer>>
    ();
  }

  Ref <IndexBuffer> VulkanVertexArray::IndexBuffer() const {
    return AGE::Ref<AGE::IndexBuffer>();
  }
} // AGE
//
// Created by alex on 3/7/23.
//


#include "agepch.h"

#include "VulkanBuffer.h"

namespace AGE {

/*
* ===============================================================
* ||                  VertexBuffer                             ||
* ===============================================================
*/

  VulkanVertexBuffer::VulkanVertexBuffer(float* vertices, uint32_t count) {

  }

  VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size) {

  }

  VulkanVertexBuffer::~VulkanVertexBuffer() {

  }

  void VulkanVertexBuffer::SetLayout(const BufferLayout& layout) {

  }

  const BufferLayout& VulkanVertexBuffer::Layout() const {
    return m_Layout;
  }

  void VulkanVertexBuffer::SetData(void* data, uint32_t size) {

  }

  void VulkanVertexBuffer::Bind() const {

  }

  void VulkanVertexBuffer::Unbind() const {

  }

  uint32_t VulkanVertexBuffer::Count() const {
    return 0;
  }

/* ===============================================================
 * ||                   IndexBuffer                             ||
 * ===============================================================
 */

  VulkanIndexBuffer::VulkanIndexBuffer(uint32_t* indices, uint32_t count) {

  }

  VulkanIndexBuffer::~VulkanIndexBuffer() {

  }

  void VulkanIndexBuffer::Bind() const {

  }

  void VulkanIndexBuffer::Unbind() const {

  }

  uint32_t VulkanIndexBuffer::Count() const {
    return 0;
  }
} // AGE
//
// Created by alex on 3/7/23.
//


#include "agepch.h"

#include "VulkanTexture.h"

namespace AGE {
  VulkanTexture2D::VulkanTexture2D(const age_string_t& filepath) {

  }

  VulkanTexture2D::VulkanTexture2D(uint32_t width, uint32_t height) {

  }

  VulkanTexture2D::VulkanTexture2D() {

  }

  void VulkanTexture2D::SetData(void* data, uint32_t size) {

  }

  uint32_t VulkanTexture2D::Width() const {
    return 0;
  }

  uint32_t VulkanTexture2D::Height() const {
    return 0;
  }

  uint32_t VulkanTexture2D::ID() const {
    return 0;
  }

  void VulkanTexture2D::Bind(uint32_t slot) const {

  }

  void VulkanTexture2D::Unbind() const {

  }

  uint32_t VulkanTexture2D::Slot() const {
    return 0;
  }

  bool VulkanTexture2D::IsCorrect() const {
    return false;
  }

  bool VulkanTexture2D::operator==(const Texture2D& other) const {
    return Texture2D::operator==(other);
  }

  Ref <Texture2D> VulkanTexture2D::ErrorTexture() {
    return AGE::Ref<VulkanTexture2D>();
  }
} // AGE
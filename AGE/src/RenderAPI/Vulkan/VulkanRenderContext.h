//
// Created by alex on 3/7/23.
//

#pragma once

#include "VulkanPlatformMaster.h"

#include "Age/Renderer/RenderContext.h"

namespace AGE {
  struct VulkanRenderContextCreateInfo {
    uint32_t WindowWidth;
    uint32_t WindowHeight;
    GLFWwindow* WindowHandle;
    const char* AppName;
  };

  class VulkanRenderContext : public RenderContext {
  public:
    VulkanRenderContext(const VulkanRenderContextCreateInfo& createInfo);
    ~VulkanRenderContext() override = default;
    void Init() override;
    void SwapBuffers() override;

  private:
    VkInstance m_VkInstance{VK_NULL_HANDLE};
    VkPhysicalDevice m_PhysicalDevice{VK_NULL_HANDLE};
    VkDevice m_LogicalDevice{VK_NULL_HANDLE};
  };

} // AGE

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
    explicit VulkanRenderContext(const VulkanRenderContextCreateInfo& createInfo);
    ~VulkanRenderContext() override;
    void Init(const VulkanRenderContextCreateInfo& createInfo);
    void SwapBuffers() override;

  private:
    void CreateInstance(const VulkanRenderContextCreateInfo& createInfo);
    void PickPhysicalDevice();

    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void SetupDebugMessenger();

    VkResult CreateDebugMessenger(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger
    );
    void DestroyDebugMessenger(
        VkInstance instance,
        VkDebugUtilsMessengerEXT messenger,
        const VkAllocationCallbacks* pAllocator);

    void Cleanup();

    bool CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions();
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    );
    uint32_t RatePhysicalDeviceSuitability(VkPhysicalDevice device);

  private:
    VkInstance m_VkInstance{VK_NULL_HANDLE};
    VkDebugUtilsMessengerEXT m_DebugMessenger{VK_NULL_HANDLE};
    VkPhysicalDevice m_PhysicalDevice{VK_NULL_HANDLE};
    VkDevice m_LogicalDevice{VK_NULL_HANDLE};
  };

} // AGE
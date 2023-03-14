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
    void RetrieveQueues();
    void CreateLogicalDevice();
    void CreateSurface(const VulkanRenderContextCreateInfo& contextCreateInfo);
    void CreateSwapChain();

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
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    uint32_t RatePhysicalDeviceSuitability(VkPhysicalDevice device);

  private:
    VkInstance m_VkInstance{VK_NULL_HANDLE};
    VkDebugUtilsMessengerEXT m_DebugMessenger{VK_NULL_HANDLE};
    VkPhysicalDevice m_PhysicalDevice{VK_NULL_HANDLE};
    VkDevice m_LogicalDevice{VK_NULL_HANDLE};
    VkQueue m_GraphicsQueue{VK_NULL_HANDLE};
    VkQueue m_PresentQueue{VK_NULL_HANDLE};
    VkSurfaceKHR m_Surface{VK_NULL_HANDLE};
    VkSwapchainKHR m_SwapChain{VK_NULL_HANDLE};
    std::vector<VkImage> m_SwapChainImages;
    VkFormat m_SwapChainFormat;
    VkExtent2D m_SwapChainExtent;
  };

} // AGE

//
// Created by alex on 3/14/23.
//

#pragma once

#include <vector>
#include <limits>
#include <algorithm>

#include "VulkanPlatformMaster.h"

namespace AGE {
  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR Capabilities;
    std::vector<VkSurfaceFormatKHR> Formats;
    std::vector<VkPresentModeKHR> PresentModes;
  };

  SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details{};

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);

    uint32_t formatCount{0};
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    details.Formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.data());

    uint32_t modeCount{0};
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, nullptr);

    details.PresentModes.resize(modeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &modeCount, details.PresentModes.data());

    return details;
  }

  VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& format: availableFormats) {
      if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return format;
      }
    }

    return availableFormats[0];
  }

  VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes) {
    for (const auto& presentMode: presentModes) {
      if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        return presentMode;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
      return capabilities.currentExtent;
    } else {
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);

      VkExtent2D actualExtent{(uint32_t)width, (uint32_t)height};

      actualExtent.width = std::clamp(
          actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width
      );

      actualExtent.height = std::clamp(
          actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height
      );

      return actualExtent;
    }
  }
} // AGE

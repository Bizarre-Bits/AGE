//
// Created by alex on 3/14/23.
//


#include "agepch.h"

#include "QueueFamilyIndices.h"

namespace AGE {
  QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndices indices{};

    uint32_t queueFamilyCount{0};
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (uint32_t i{0}; i < queueFamilyCount; ++i) {
      if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
        indices.GraphicsFamily = i;
      }
      VkBool32 presentSupport{false};
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
      if (presentSupport) {
        indices.PresentFamily = i;
      }

      if (indices.IsComplete())
        break;
    }

    return indices;
  }
} // AGE
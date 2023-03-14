//
// Created by alex on 3/14/23.
//

#pragma once

#include <optional>

#include "VulkanPlatformMaster.h"

namespace AGE {
  struct QueueFamilyIndices {
    std::optional<uint32_t> GraphicsFamily;
    std::optional<uint32_t> PresentFamily;

    inline bool IsComplete() const {
      return GraphicsFamily.has_value() && PresentFamily.has_value();
    }
  };

  QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
} // AGE

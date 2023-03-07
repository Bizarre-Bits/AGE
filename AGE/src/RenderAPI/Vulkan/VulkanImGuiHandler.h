//
// Created by alex on 3/7/23.
//

#pragma once

#include "Age/Renderer/ImGuiHandler.h"

namespace AGE {
  class VulkanImGuiHandler : public ImGuiHandler_PlatformImpl {
  public:
    void InitImGui() override;
    void ShutDownImGui() override;
    void BeginFrame() override;
    void EndFrame() override;
  };
} // AGE

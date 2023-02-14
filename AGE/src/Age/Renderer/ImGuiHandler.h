//
// Created by alex on 2/5/23.
//

#pragma once

#include "Age/Core/Core.h"
namespace AGE {
  class ImGuiHandler_PlatformImpl {
  public:
    virtual void InitImGui() = 0;
    virtual void ShutDownImGui() = 0;
    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;
  };

  class ImGuiHandler {
  public:
    static void InitImGui() {
      s_ImGuiPlatformAPI->InitImGui();
    }

    static void ShutDownImGui() {
      s_ImGuiPlatformAPI->ShutDownImGui();
    }

    static void BeginFrame() {
      s_ImGuiPlatformAPI->BeginFrame();
    }

    static void EndFrame() {
      s_ImGuiPlatformAPI->EndFrame();
    }

    static Ref<ImGuiHandler_PlatformImpl> CreatePlatformAPI();

  private:
    static Ref<ImGuiHandler_PlatformImpl> s_ImGuiPlatformAPI;
  };
}

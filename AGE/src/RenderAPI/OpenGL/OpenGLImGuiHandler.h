//
// Created by alex on 2/5/23.
//

#pragma once

#include "Age/Core/ImGuiBuild.h"
#include "Age/Renderer/ImGuiHandler.h"
#include "OpenGLMaster.h"

namespace AGE {

  class OpenGLImGuiHandler : public ImGuiHandler_PlatformImpl {
  public:
    virtual void InitImGui() override;
    virtual void ShutDownImGui() override;
    virtual void BeginFrame() override;
    virtual void EndFrame() override;
  private:
    static bool s_IsInitialized;
  };

} // AGE

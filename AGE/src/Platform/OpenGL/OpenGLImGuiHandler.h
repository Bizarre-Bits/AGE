//
// Created by alex on 2/5/23.
//

#ifndef AGE_OPENGLIMGUIHANDLER_H
#define AGE_OPENGLIMGUIHANDLER_H

#include "OpenGLPlatform.h"
#include "Age/Renderer/ImGuiHandler.h"
#include "Age/Core/ImGuiBuild.h"

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

#endif //AGE_OPENGLIMGUIHANDLER_H

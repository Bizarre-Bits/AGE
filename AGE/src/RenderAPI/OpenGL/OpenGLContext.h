//
// Created by alex on 08.09.22.
//

#pragma once

#include "Age/Renderer/RenderContext.h"
#include "RenderAPI/OpenGL/OpenGLMaster.h"

namespace AGE {
  class OpenGLContext : public RenderContext {
  public:
    explicit OpenGLContext(GLFWwindow* windowHandle);

    virtual void Init() override;
    virtual void SwapBuffers() override;

  private:
    GLFWwindow* m_WindowHandle;
  };

}// namespace AGE

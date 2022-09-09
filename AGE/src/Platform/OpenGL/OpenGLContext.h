//
// Created by alex on 08.09.22.
//

#ifndef AGE_OPENGLCONTEXT_H
#define AGE_OPENGLCONTEXT_H

#include "Age/Renderer/Context.h"
#include "Platform/OpenGL/OpenGLPlatform.h"

namespace AGE {
  class OpenGLContext : public Context {
  public:
    explicit OpenGLContext(GLFWwindow* windowHandle);

    virtual void Init() override;
    virtual void SwapBuffers() override;

  private:
    GLFWwindow* m_WindowHandle;
  };

}// namespace AGE

#endif//AGE_OPENGLCONTEXT_H

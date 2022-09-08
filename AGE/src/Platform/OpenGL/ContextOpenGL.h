//
// Created by alex on 08.09.22.
//

#ifndef AGE_CONTEXTOPENGL_H
#define AGE_CONTEXTOPENGL_H

#include "Age/Renderer/Context.h"
#include "Platform/OpenGL/PlatformGL.h"

namespace AGE {
  class ContextOpenGL : public Context {
  public:
    explicit ContextOpenGL(GLFWwindow* windowHandle);

    virtual void Init() override;
    virtual void SwapBuffers() override;

  private:
    GLFWwindow* m_WindowHandle;
  };

}// namespace AGE

#endif//AGE_CONTEXTOPENGL_H

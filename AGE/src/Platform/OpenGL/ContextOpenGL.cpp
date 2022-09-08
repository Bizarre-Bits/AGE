//
// Created by alex on 08.09.22.
//

#include "agepch.h"

#include "Platform/OpenGL/PlatformGL.h"

#include "Age/Debug/Assert.h"
#include "ContextOpenGL.h"

namespace AGE {
  ContextOpenGL::ContextOpenGL(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {}

  void ContextOpenGL::Init() {
    glfwMakeContextCurrent(m_WindowHandle);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    AGE_CORE_ASSERT(status, "Failed to initialize Glad");
    AGE_CORE_INFO("Initialized Glad");
  }

  void ContextOpenGL::SwapBuffers() {
    glfwSwapBuffers(m_WindowHandle);
  }
}// namespace AGE
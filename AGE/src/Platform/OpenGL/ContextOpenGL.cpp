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
    AGE_CORE_TRACE("Initialized Glad");

    AGE_CORE_INFO("OpenGL info:");
    AGE_CORE_INFO("   Vendor: {0}", glGetString(GL_VENDOR));
    AGE_CORE_INFO("   Renderer: {0}", glGetString(GL_RENDERER));
    AGE_CORE_INFO("   Version: {0}", glGetString(GL_VERSION));
  }

  void ContextOpenGL::SwapBuffers() {
    glfwSwapBuffers(m_WindowHandle);
  }
}// namespace AGE
//
// Created by alex on 08.09.22.
//

#include "agepch.h"
#include "RenderAPI/OpenGL/OpenGLMaster.h"

#include "OpenGLContext.h"

namespace AGE {
  OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle) {}

  void OpenGLContext::Init() {
    AGE_PROFILE_FUNCTION();

    glfwMakeContextCurrent(m_WindowHandle);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    AGE_CORE_ASSERT(status, "Failed to initialize Glad");
    AGE_CORE_TRACE("Initialized Glad");

    AGE_CORE_INFO("OpenGL info:");
    AGE_CORE_INFO("   Vendor: {0}", reinterpret_cast<const char*>((glGetString(GL_VENDOR))));
    AGE_CORE_INFO("   Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    AGE_CORE_INFO("   Version: {0}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
  }

  void OpenGLContext::SwapBuffers() {
    AGE_PROFILE_FUNCTION();

    glfwSwapBuffers(m_WindowHandle);
  }
}// namespace AGE

//
// Created by alex on 3/7/23.
//


#include "agepch.h"

#include "VulkanWindow.h"
#include "VulkanRenderContext.h"

namespace AGE {
  VulkanWindow::VulkanWindow(const WindowProps& props) {
    Init(props);
  }

  VulkanWindow::~VulkanWindow() {
    Dispose();
  }

  void VulkanWindow::OnUpdate() {
    glfwPollEvents();
  }

  void VulkanWindow::Clear() {

  }

  age_string_t VulkanWindow::Title() const {
    return age_string_t();
  }

  unsigned int VulkanWindow::Width() const {
    return 0;
  }

  unsigned int VulkanWindow::Height() const {
    return 0;
  }

  void VulkanWindow::EventCallback(const Window::EventCallbackFn& callback) {

  }

  void VulkanWindow::SetVSync(bool enabled) {

  }

  bool VulkanWindow::VSync() const {
    return false;
  }

  void VulkanWindow::SetTitle(const age_string_t& title) {

  }

  void* VulkanWindow::NativeWindow() const {
    return nullptr;
  }

  void VulkanWindow::Init(const WindowProps& props) {
    if (!glfwInit()) {
      AGE_CORE_CRITICAL("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_WindowHandle = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);

    if (!m_WindowHandle) {
      AGE_CORE_CRITICAL("Failed to create a window");
    }

    VulkanRenderContextCreateInfo contextCreateInfo{};
    contextCreateInfo.WindowHandle = m_WindowHandle;
    contextCreateInfo.WindowWidth = props.Width;
    contextCreateInfo.WindowHeight = props.Height;
    contextCreateInfo.AppName = props.Title.c_str();

    m_Context = CreateScope<VulkanRenderContext>(contextCreateInfo);
  }

  void VulkanWindow::Dispose() {
    glfwDestroyWindow(m_WindowHandle);
    glfwTerminate();
  }
} // AGE
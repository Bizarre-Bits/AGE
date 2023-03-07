//
// Created by alex on 3/7/23.
//

#pragma once

#include <GLFW/glfw3.h>
#include "Age/Core/Window.h"

namespace AGE {
  class VulkanWindow : public Window {
  public:
    explicit VulkanWindow(const WindowProps& props);
    ~VulkanWindow() override;

    void OnUpdate() override;
    void Clear() override;

    [[nodiscard]] age_string_t Title() const override;
    [[nodiscard]] unsigned int Width() const override;
    [[nodiscard]] unsigned int Height() const override;

    void EventCallback(const EventCallbackFn& callback) override;

    void SetVSync(bool enabled) override;
    [[nodiscard]] bool VSync() const override;

    void SetTitle(const age_string_t& title) override;

    [[nodiscard]] void* NativeWindow() const override;

  private:
    void Init(const WindowProps& props);
    void Dispose();

  private:
    GLFWwindow* m_WindowHandle{nullptr};
  };
} // AGE

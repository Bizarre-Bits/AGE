//
// Created by alex on 28.08.22.
//

#ifndef AGE_OPENGLWINDOW_H
#define AGE_OPENGLWINDOW_H

#include "Age/Core/Core.h"
#include "Age/Core/Window.h"

#include "Platform/OpenGL/OpenGLPlatform.h"

namespace AGE {
  class OpenGLWindow : public Window {
  public:
    OpenGLWindow(const WindowProps& props);
    virtual ~OpenGLWindow();

    virtual void OnUpdate() override;
    virtual void Clear() override;

    inline age_string_t Title() const override { return m_Data.Title; }
    inline unsigned int Width() const override { return m_Data.Width; }
    inline unsigned int Height() const override { return m_Data.Height; }

    void SetVSync(bool enabled) override;
    bool VSync() const override;

    virtual void* NativeWindow() const override;

    virtual void EventCallback(const EventCallbackFn& callback) override;

  private:
    virtual void Init(const WindowProps& props);
    virtual void Destroy();

  private:
    GLFWwindow* m_Window;

    struct WindowData {
      age_string_t Title;
      unsigned int Width, Height;
      bool VSync;

      EventCallbackFn EventCallback;
    } m_Data;

    static bool s_GLFWInitialized;
    static bool s_ImGuiInitialized;
  };
}// namespace AGE

#endif//AGE_OPENGLWINDOW_H

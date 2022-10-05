//
// Created by alex on 28.08.22.
//

#include "agepch.h"

#include "Platform/OpenGL/OpenGLWindow.h"

#include "Age/Debug/Assert.h"
#include "Age/Events/KeyEvent.h"
#include "Age/Events/MouseEvent.h"
#include "Age/Events/WindowEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace AGE {
  bool OpenGLWindow::s_GLFWInitialized{false};

  void GLFWErrorCallback(int error, const char* description) {
    AGE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
  }

  Window* Window::Create(const AGE::WindowProps& props) {
    return new OpenGLWindow(props);
  }

  OpenGLWindow::OpenGLWindow(const WindowProps& props) {
    Init(props);
  }

  OpenGLWindow::~OpenGLWindow() {
    Destroy();
  }

  void OpenGLWindow::Init(const WindowProps& props) {
    m_Data.Title  = props.Title;
    m_Data.Width  = props.Width;
    m_Data.Height = props.Height;

    AGE_CORE_TRACE("Creating Window \"{0}\" ({1}, {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized) {
      int success = glfwInit();
      AGE_CORE_ASSERT(success, "Could not initialize GLFW");
      s_GLFWInitialized = true;
      AGE_CORE_TRACE("Initialized GLFW: {0}", glfwGetVersionString());
      glfwSetErrorCallback(GLFWErrorCallback);
    }

    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), NULL, NULL);
    glfwSetWindowUserPointer(m_Window, &m_Data);

    m_Context = new OpenGLContext{m_Window};
    m_Context->Init();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    SetVSync(true);

    AGE_CORE_TRACE("Created Window \"{0}\" ({1}, {2})", props.Title, props.Width, props.Height);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
      auto* data = (WindowData*)glfwGetWindowUserPointer(window);

      WindowResizeEvent event(width, height);
      data->EventCallback(event);

      data->Width  = width;
      data->Height = height;
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
      auto data = (WindowData*)glfwGetWindowUserPointer(window);

      WindowCloseEvent event;
      data->EventCallback(event);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int keycode, int scancode, int action, int mods) {
      auto data = (WindowData*)glfwGetWindowUserPointer(window);
      switch (action) {
        case GLFW_PRESS: {
          KeyPressedEvent event(keycode, false);
          data->EventCallback(event);
          break;
        }
        case GLFW_RELEASE: {
          KeyReleasedEvent event(keycode);
          data->EventCallback(event);
          break;
        }
        case GLFW_REPEAT: {
          KeyPressedEvent event(keycode, true);
          data->EventCallback(event);
          break;
        }
        default: break;
      }
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
      auto data = (WindowData*)glfwGetWindowUserPointer(window);

      switch (action) {
        case GLFW_PRESS: {
          MouseButtonPressedEvent event(button);
          data->EventCallback(event);
          break;
        }
        case GLFW_RELEASE: {
          MouseButtonReleasedEvent event(button);
          data->EventCallback(event);
          break;
        }
        default: break;
      }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
      auto data = (WindowData*)glfwGetWindowUserPointer(window);
      KeyTypedEvent event{static_cast<unsigned short>(keycode)};
      data->EventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y) {
      auto data = (WindowData*)glfwGetWindowUserPointer(window);

      MouseMovedEvent event((float)x, (float)y);
      data->EventCallback(event);
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
      auto data = (WindowData*)glfwGetWindowUserPointer(window);

      MouseScrolledEvent event((float)xOffset, (float)yOffset);
      data->EventCallback(event);
    });
  }

  void OpenGLWindow::OnUpdate() {
    glfwPollEvents();

    m_Context->SwapBuffers();
  }

  void OpenGLWindow::SetVSync(bool enabled) {
    if (enabled) {
      glfwSwapInterval(1);
    } else {
      glfwSwapInterval(0);
    }
    m_Data.VSync = enabled;
  }

  bool OpenGLWindow::VSync() const {
    return m_Data.VSync;
  }

  void OpenGLWindow::EventCallback(const Window::EventCallbackFn& callback) {
    m_Data.EventCallback = callback;
  }

  void OpenGLWindow::Destroy() {
    glfwDestroyWindow(m_Window);
  }
  void* OpenGLWindow::NativeWindow() const {
    return m_Window;
  }
  void OpenGLWindow::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
}// namespace AGE
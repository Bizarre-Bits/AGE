//
// Created by alex on 28.08.22.
//

#include "agepch.h"

#include "Platform/OpenGL/OpenGLWindow.h"

#include "Age/Debug/Assert.h"
#include "Age/Events/KeyEvent.h"
#include "Age/Events/MouseEvent.h"
#include "Age/Events/WindowEvent.h"

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
    m_Data.Title  = props.title;
    m_Data.Width  = props.width;
    m_Data.Height = props.height;

    AGE_CORE_INFO("Creating Window \"{0}\" ({1}, {2})", props.title, props.width, props.height);

    if (!s_GLFWInitialized) {
      int success = glfwInit();
      AGE_CORE_ASSERT(success, "Could not initialize GLFW");
      s_GLFWInitialized = true;
      AGE_CORE_INFO("Initialized GLFW: {0}", glfwGetVersionString());
      glfwSetErrorCallback(GLFWErrorCallback);
    }

    m_Window = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    glfwMakeContextCurrent(m_Window);

    //Initialize GLAD
    {
      int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
      AGE_CORE_ASSERT(status, "Failed to initialize Glad");
      AGE_CORE_INFO("Initialized Glad");
    }

    SetVSync(true);

    AGE_CORE_INFO("Created Window \"{0}\" ({1}, {2})", props.title, props.width, props.height);

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

    //Swaps buffers
    glfwSwapBuffers(m_Window);

    // And starts the next frame
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
}// namespace AGE
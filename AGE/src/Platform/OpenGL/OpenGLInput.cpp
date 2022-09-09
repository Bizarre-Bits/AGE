//
// Created by alex on 06.09.22.
//

#include "agepch.h"

#include "Platform/OpenGL/OpenGLPlatform.h"

#include "Age/Core/Application.h"
#include "Platform/OpenGL/OpenGLInput.h"


namespace AGE {
  Input* Input::s_Instance = new OpenGLInput;

  bool OpenGLInput::IsKeyPressedImpl(KeyCode keycode) {
    auto window = (GLFWwindow*)Application::Instance()->Window()->NativeWindow();

    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool OpenGLInput::IsMousePressedImpl(MouseCode mousecode) {
    auto window = (GLFWwindow*)Application::Instance()->Window()->NativeWindow();
    int state = glfwGetMouseButton(window, mousecode);
    return state == GLFW_PRESS;
  }

  std::pair<float, float> OpenGLInput::MousePosImpl() {
    auto window = (GLFWwindow*)Application::Instance()->Window()->NativeWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {(float)xpos, (float)ypos};
  }

  float OpenGLInput::MouseXImpl() {
    auto[x,y] = MousePosImpl();
    return x;
  }

  float OpenGLInput::MouseYImpl() {
    auto[x, y] = MousePosImpl();
    return y;
  }
}// namespace AGE
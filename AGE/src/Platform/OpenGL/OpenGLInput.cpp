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
    AGE_PROFILE_FUNCTION();

    auto window = (GLFWwindow*)Application::Instance()->Window().NativeWindow();

    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool OpenGLInput::IsMousePressedImpl(MouseCode mousecode) {
    AGE_PROFILE_FUNCTION();

    auto window = (GLFWwindow*)Application::Instance()->Window().NativeWindow();
    int state = glfwGetMouseButton(window, mousecode);
    return state == GLFW_PRESS;
  }

  std::pair<float, float> OpenGLInput::MousePosImpl() {
    AGE_PROFILE_FUNCTION();

    auto window = (GLFWwindow*)Application::Instance()->Window().NativeWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {(float)xpos, (float)ypos};
  }

  float OpenGLInput::MouseXImpl() {
    AGE_PROFILE_FUNCTION();

    auto[x,y] = MousePosImpl();
    return x;
  }

  float OpenGLInput::MouseYImpl() {
    AGE_PROFILE_FUNCTION();

    auto[x, y] = MousePosImpl();
    return y;
  }
}// namespace AGE

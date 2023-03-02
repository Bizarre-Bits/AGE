//
// Created by alex on 2/14/23.
//

#include "agepch.h"

#include "Age/Core/Input.h"
#include "agepch.h"
#include <Age/Core/Application.h>

#include "RenderAPI/OpenGL/OpenGLMaster.h"

namespace AGE {
  bool Input::IsKeyPressed(KeyCode key) {
    AGE_PROFILE_FUNCTION();

    auto window = (GLFWwindow*)Application::Instance()->Window().NativeWindow();

    auto state = glfwGetKey(window, key);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
  }

  bool Input::IsMouseButtonPressed(MouseCode button) {
    AGE_PROFILE_FUNCTION();

    auto window = (GLFWwindow*)Application::Instance()->Window().NativeWindow();
    int state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
  }

  std::pair<float, float> Input::MousePosition() {
    AGE_PROFILE_FUNCTION();

    auto window = (GLFWwindow*)Application::Instance()->Window().NativeWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {(float)xpos, (float)ypos};
  }

  float Input::MouseX() {
    AGE_PROFILE_FUNCTION();

    auto[x,y] = MousePosition();
    return x;
  }

  float Input::MouseY() {
    AGE_PROFILE_FUNCTION();

    auto[x, y] = MousePosition();
    return y;
  }
}// namespace AGE

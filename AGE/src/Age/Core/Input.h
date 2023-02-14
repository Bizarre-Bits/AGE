//
// Created by alex on 06.09.22.
//

#pragma once

#include <glm/glm.hpp>

#include "Age/Core/Core.h"
#include "Age/Core/KeyCodes.h"
#include "Age/Core/MouseCodes.h"

namespace AGE {
  class Input {
  public:
    static bool IsKeyPressed(KeyCode key);
    static bool IsMouseButtonPressed(MouseCode button);
    static std::pair<float, float> MousePosition();
    static float MouseX();
    static float MouseY();
  };
}// namespace AGE

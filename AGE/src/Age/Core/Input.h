//
// Created by alex on 06.09.22.
//

#ifndef AGE_INPUT_H
#define AGE_INPUT_H

#include "Age/Core/Core.h"
#include "Age/Core/KeyCodes.h"
#include "Age/Core/MouseCodes.h"

namespace AGE {
  class  Input {
  public:
    inline static bool IsKeyPressed(KeyCode keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
    inline static bool IsMousePressed(MouseCode mousecode) { return s_Instance->IsMousePressedImpl(mousecode); }
    inline static std::pair<float, float> MousePos() { return s_Instance->MousePosImpl(); }
    inline static float MouseX() { return s_Instance->MouseYImpl(); }
    inline static float MouseY() { return s_Instance->MouseXImpl(); }

  protected:
    virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
    virtual bool IsMousePressedImpl(MouseCode mousecode) = 0;
    virtual std::pair<float, float> MousePosImpl() = 0;
    virtual float MouseXImpl() = 0;
    virtual float MouseYImpl() = 0;

  private:
    static Input* s_Instance;
  };
}// namespace AGE

#endif//AGE_INPUT_H

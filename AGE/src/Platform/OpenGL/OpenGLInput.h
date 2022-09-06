//
// Created by alex on 06.09.22.
//

#ifndef AGE_OPENGLINPUT_H
#define AGE_OPENGLINPUT_H

#include "Age/Core/Core.h"
#include "Age/Core/Input.h"

namespace AGE {

  class DLL_PUBLIC OpenGLInput : public Input {

  protected:
    virtual bool IsKeyPressedImpl(KeyCode keycode) override;
    virtual bool IsMousePressedImpl(MouseCode mousecode) override;
    virtual float MouseXImpl() override;
    virtual float MouseYImpl() override;
    virtual std::pair<float, float> MousePosImpl() override;
  };

}// namespace AGE

#endif//AGE_OPENGLINPUT_H

//
// Created by alex on 08.09.22.
//

#ifndef AGE_SHADER_H
#define AGE_SHADER_H

#include "Age/Core/Core.h"

namespace AGE {
  class Shader {
  public:
    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
  };

} // AGE

#endif //AGE_SHADER_H

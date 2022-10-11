//
// Created by alex on 08.09.22.
//

#ifndef AGE_SHADER_H
#define AGE_SHADER_H

#include "Age/Core/Core.h"

#include <glm/glm.hpp>

namespace AGE {
  class Shader {
  public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    static Shader* Create(const age_string_t& vertexSrc, const age_string_t& fragmentSrc);
  };

} // AGE

#endif //AGE_SHADER_H

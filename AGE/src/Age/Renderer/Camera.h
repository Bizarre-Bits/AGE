//
// Created by alex on 2/16/23.
//

#pragma once

#include <glm/glm.hpp>

namespace AGE {
  class Camera {
  public:
    Camera() = default;
    Camera(const glm::mat4& projection) : m_Projection(projection) {}
    virtual ~Camera() = default;

    inline const glm::mat4& Projection() const { return m_Projection; }

  protected:
    glm::mat4 m_Projection;
  };
}// namespace AGE

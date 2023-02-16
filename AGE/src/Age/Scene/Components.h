//
// Created by alex on 2/15/23.
//

#pragma once

#include <glm/glm.hpp>

namespace AGE {
  struct TransformComponent {
    glm::mat4 Transform{1.0f};

    TransformComponent()                          = default;
    TransformComponent(const TransformComponent&) = default;
    explicit TransformComponent(const glm::mat4& transform) : Transform{transform} {}

    operator const glm::mat4&() { return Transform; }
  };

  struct SpriteComponent {
    glm::vec4 Tint{1.0f};
  };
}// namespace AGE

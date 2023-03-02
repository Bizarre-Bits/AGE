//
// Created by alex on 2/23/23.
//

#pragma once

#include <glm/glm.hpp>

namespace AGE::MathUtils {
  bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
}

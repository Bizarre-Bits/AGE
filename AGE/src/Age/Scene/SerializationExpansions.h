//
// Created by alex on 2/21/23.
//

#pragma once

#include <glm/glm.hpp>
#include <yaml-cpp/yaml.h>

namespace AGE {
  YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v);
  YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v);
  YAML::Emitter& operator<<(YAML::Emitter& out, const glm::mat4& m);
}// namespace AGE

namespace YAML {
  template<>
  struct convert<glm::vec3> {
    static Node encode(const glm::vec3& rhs);
    static bool decode(const Node& node, glm::vec3& rhs);
  };

  template<>
  struct convert<glm::vec4> {
    static Node encode(const glm::vec4& rhs);
    static bool decode(const Node& node, glm::vec4& rhs);
  };
}// namespace YAML
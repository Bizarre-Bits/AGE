//
// Created by alex on 2/21/23.
//

#include "agepch.h"

#include "SerializationExpansions.h"

namespace AGE {
  YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
    return out;
  }

  YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v) {
    out << YAML::Flow;
    out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
    return out;
  }

  YAML::Emitter& operator<<(YAML::Emitter& out, const glm::mat4& m) {
    out << YAML::BeginSeq;
    out << m[0] << m[1] << m[2] << m[3];
    out << YAML::EndSeq;
    return out;
  }
}// namespace AGE

namespace YAML {
  Node convert<glm::vec3>::encode(const glm::vec3& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    return node;
  }

  bool convert<glm::vec3>::decode(const Node& node, glm::vec3& rhs) {
    if (!node.IsSequence() || node.size() != 3)
      return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    return true;
  }

  Node convert<glm::vec4>::encode(const glm::vec4& rhs) {
    Node node;
    node.push_back(rhs.x);
    node.push_back(rhs.y);
    node.push_back(rhs.z);
    node.push_back(rhs.w);
    return node;
  }

  bool convert<glm::vec4>::decode(const Node& node, glm::vec4& rhs) {
    if (!node.IsSequence() || node.size() != 4)
      return false;

    rhs.x = node[0].as<float>();
    rhs.y = node[1].as<float>();
    rhs.z = node[2].as<float>();
    rhs.w = node[3].as<float>();
    return true;
  }
}// namespace YAML

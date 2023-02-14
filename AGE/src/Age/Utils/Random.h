//
// Created by alex on 1/26/23.
//

#pragma once

#include <random>
#include <cinttypes>

#include <glm/glm.hpp>

namespace AGE {

  class Random {
  public:
    static float Float()
    {
      return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint_fast32_t>::max();
    }
    static float Float(float min, float max) {
      return Float() * (max - min) + min;
    }

    static uint32_t UInt() {
      return s_Distribution(s_RandomEngine);
    }
    static uint32_t UInt(uint32_t min, uint32_t max) {
      return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
    }

    static glm::vec3 Vec3() {
      return {Float(), Float(), Float()};
    }
    static glm::vec3 Vec3(float min, float max) {
      return {Float(min, max), Float(min, max), Float(min, max)};
    }

    static glm::vec2 Vec2() {
      return {Float(), Float()};
    }
    static glm::vec2 Vec2(float min, float max) {
      return {Float(min, max), Float(min, max)};
    }

    static glm::vec3 InUnitSphere() {
      return glm::normalize(Vec3(-1.0f, 1.0f));
    }

  private:
    static thread_local std::mt19937                                             s_RandomEngine;
    static thread_local std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
  };

} // AGE

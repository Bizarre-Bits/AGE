//
// Created by alex on 1/26/23.
//

#include "agepch.h"

#include "Random.h"

namespace AGE {
  thread_local std::mt19937                                             Random::s_RandomEngine{std::random_device()()};
  thread_local std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution{};
} // AGE

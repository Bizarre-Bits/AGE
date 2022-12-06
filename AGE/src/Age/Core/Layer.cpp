//
// Created by alex on 29.08.22.
//
#include "agepch.h"

#include "Layer.h"

namespace AGE {
  Layer::Layer(const age_string_t& debugName) : m_DebugName(debugName) {}
  Layer::~Layer() = default;
}// namespace AGE

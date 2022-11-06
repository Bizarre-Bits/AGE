//
// Created by alex on 29.08.22.
//
#include "agepch.h"

#include "LayerStack.h"

namespace AGE {
  LayerStack::LayerStack() : m_Layers(), m_InsertIndex{0} {}

  LayerStack::~LayerStack() {
    AGE_PROFILE_FUNCTION();

    for (Layer* l: m_Layers) {
      delete l;
    }
  }

  void LayerStack::PushLayer(Layer* layer) {
    AGE_PROFILE_FUNCTION();

    m_Layers.emplace(begin() + m_InsertIndex, layer);
    m_InsertIndex++;
  }

  void LayerStack::PushOverlay(Layer* layer) {
    AGE_PROFILE_FUNCTION();

    m_Layers.emplace_back(layer);
  }

  void LayerStack::PopLayer(Layer* layer) {
    AGE_PROFILE_FUNCTION();

    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end()) {
      layer->OnDetach();
      m_Layers.erase(it);
      m_InsertIndex--;
    }
  }

  void LayerStack::PopOverlay(Layer* layer) {
    AGE_PROFILE_FUNCTION();

    auto it = std::find(m_Layers.begin() + m_InsertIndex, m_Layers.end(), layer);
    if (it != m_Layers.end())
      m_Layers.erase(it);
  }

}// namespace AGE
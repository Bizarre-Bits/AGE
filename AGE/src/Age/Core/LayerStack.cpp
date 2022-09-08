//
// Created by alex on 29.08.22.
//
#include "agepch.h"

#include "LayerStack.h"

namespace AGE {
  LayerStack::LayerStack() {
    m_LayerInsert = m_Layers.begin();
  }

  LayerStack::~LayerStack() {
    for (Layer* l: m_Layers) {
      delete l;
    }
  }

  void LayerStack::PushLayer(Layer* layer) {
    m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
  }

  void LayerStack::PushOverlay(Layer* layer) {
    m_Layers.emplace_back(layer);
  }

  void LayerStack::PopLayer(Layer* layer) {
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end()) {
      m_Layers.erase(it);
      m_LayerInsert--;
    }
  }

  void LayerStack::PopOverlay(Layer* layer) {
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end())
      m_Layers.erase(it);
  }

}// namespace AGE
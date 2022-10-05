//
// Created by alex on 29.08.22.
//
#include "agepch.h"

#include "LayerStack.h"

namespace AGE {

  LayerStack::~LayerStack() {
    for (Layer* l: m_Layers) {
      delete l;
    }
  }

  void LayerStack::PushLayer(Layer* layer) {
    m_Layers.emplace(begin() + m_InsertIndex, layer);
    m_InsertIndex++;
  }

  void LayerStack::PushOverlay(Layer* layer) {
    m_Layers.emplace_back(layer);
  }

  void LayerStack::PopLayer(Layer* layer) {
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end()) {
      layer->OnDetach();
      m_Layers.erase(it);
      m_InsertIndex--;
    }
  }

  void LayerStack::PopOverlay(Layer* layer) {
    auto it = std::find(m_Layers.begin() + m_InsertIndex, m_Layers.end(), layer);
    if (it != m_Layers.end())
      m_Layers.erase(it);
  }

}// namespace AGE
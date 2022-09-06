//
// Created by alex on 29.08.22.
//

#ifndef AGE_LAYERSTACK_H
#define AGE_LAYERSTACK_H

#include <vector>

#include "Age/Core/Core.h"
#include "Age/Core/Layer.h"

namespace AGE {

  class LayerStack {
  public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    void PopLayer(Layer* layer);
    void PopOverlay(Layer* layer);

    inline std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
    inline std::vector<Layer*>::iterator end() { return m_Layers.end(); }

  private:
    std::vector<Layer*> m_Layers;
    std::vector<Layer*>::iterator m_LayerInsert;
  };

}// namespace AGE

#endif//AGE_LAYERSTACK_H

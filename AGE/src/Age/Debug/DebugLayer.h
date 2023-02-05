//
// Created by alex on 08.09.22.
//

#ifndef AGE_DEBUGLAYER_H
#define AGE_DEBUGLAYER_H

#include "Age/Core/Layer.h"

namespace AGE {

  class FpsPlot {
  public:
    FpsPlot();
    void InsertNewFps(float fps);
    void Render();

    float LastUpdateSeconds{0};
    float RefreshTimeSeconds{0.25f};

  private:
    std::array<float, 60> m_Data;
    std::size_t           m_NextIndex{0};
    float                 m_MaxValue{0};
    float                 m_MinValue{std::numeric_limits<float>::max()};
  };

  class DebugLayer : public Layer {
  public:
    DebugLayer();
    virtual void OnUiRender(Timestep ts) override;

  private:
    FpsPlot m_FpsPlot;
  };

}// namespace AGE

#endif//AGE_DEBUGLAYER_H

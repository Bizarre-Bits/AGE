//
// Created by alex on 08.09.22.
//

#ifndef AGE_DEBUGLAYER_H
#define AGE_DEBUGLAYER_H

#include "Age/ImGui/ImGuiLayer.h"

namespace AGE {

  class FpsPlot {
  public:
    FpsPlot();
    void InsertNewFps(float fps);
    void Render();

    float LastUpdate{0};
    float RefreshTime{0.25f};

  private:
    std::array<float, 60> m_Data;
    std::size_t           m_LastInsertIndex{0};
    float                 m_MaxValue{0};
    float                 m_MinValue{FLT_MAX};
  };

  class DebugLayer : public ImGuiLayer {
  public:
    DebugLayer();
    virtual void OnUpdate(Timestep ts) override;

  private:
    FpsPlot m_FpsPlot;
  };

}// namespace AGE

#endif//AGE_DEBUGLAYER_H

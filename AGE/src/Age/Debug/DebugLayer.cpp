//
// Created by alex on 08.09.22.
//

#include "agepch.h"

#include "DebugLayer.h"

#include "Age/Core/Window.h"
#include "Age/Core/Application.h"
#include "Age/Core/Input.h"

namespace AGE {
  DebugLayer::DebugLayer() : ImGuiLayer("Debug Layer") {}

  void DebugLayer::OnUpdate(Timestep ts) {
    AGE_PROFILE_FUNCTION();

    constexpr float fpsTextUpdateThresholdSeconds{0.1f};
    static float lastFpsUpdateSeconds{0.0f};
    static float lastFps{1000.0f / ts.Milliseconds()};

    lastFpsUpdateSeconds += ts;
    if(lastFpsUpdateSeconds >= fpsTextUpdateThresholdSeconds) {
      lastFpsUpdateSeconds = 0.0f;
      lastFps = 1000.0f / ts.Milliseconds();
    }

    m_FpsPlot.LastUpdateSeconds += ts.Seconds();

    if (m_FpsPlot.LastUpdateSeconds >= m_FpsPlot.RefreshTimeSeconds) {
      m_FpsPlot.LastUpdateSeconds = 0;
      m_FpsPlot.InsertNewFps(1000.0f / ts.Milliseconds());
    }

    ImGui::Begin("Debug");
    Scope<Window>& window = Application::Instance()->Window();
    ImGui::Text(
        "MainWindow: %s (%i, %i)", window->Title().c_str(), (int)window->Width(),
        (int)window->Height());
    ImGui::Text("FPS: %.2f", lastFps);
    ImGui::Text("Delta Time: %.2fms", ts.Milliseconds());

    m_FpsPlot.Render();

    auto [mX, mY] = Input::MousePos();
    ImGui::Text("Mouse Position: %i, %i", (int)mX, (int)mY);
    ImGui::End();
  }

  // FPS Plot

  FpsPlot::FpsPlot() {
    AGE_PROFILE_FUNCTION();

    m_Data.fill(0.0f);
  }

  void FpsPlot::InsertNewFps(float fps) {
    AGE_PROFILE_FUNCTION();

    if (fps > m_MaxValue) {
      m_MaxValue = fps;
    }
    if (fps < m_MinValue) {
      m_MinValue = fps;
    }

    if (m_NextIndex >= m_Data.max_size()) {
      for (std::size_t i = 0; i < m_Data.max_size() - 1; i++) {
        m_Data[i] = m_Data[i + 1];
      }

      m_Data[m_Data.max_size() - 1] = fps;
    } else {
      m_Data[m_NextIndex] = fps;
      m_NextIndex++;
    }
  }

  void FpsPlot::Render() {
    AGE_PROFILE_FUNCTION();

    float scaleMax = *std::max_element(m_Data.begin(), m_Data.end());
    ImGui::Text("Max FPS: %.2f", m_MaxValue);
    ImGui::Text("Min FPS: %.2f", m_MinValue);


    ImGui::SliderFloat("Refresh Rate(sec)", &RefreshTimeSeconds, 0.0f, 5.0f);
    ImGui::PlotLines(
        "FPS",
        m_Data.data(),
        (int)m_Data.max_size(),
        0,
        nullptr,
        -0.1f,
        scaleMax,
        ImVec2(0, 50)
    );
  }
}// namespace AGE

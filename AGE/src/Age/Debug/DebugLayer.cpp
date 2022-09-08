//
// Created by alex on 08.09.22.
//

#include "agepch.h"

#include "DebugLayer.h"

#include "Age/Core/Window.h"
#include "Age/Core/Application.h"
#include "Age/Core/Input.h"

namespace AGE {
  void DebugLayer::OnUpdate() {
    ImGui::Begin("Debug");
    Window* window = Application::Instance()->Window();
    ImGui::Text("MainWindow: %s (%i, %i)", window->Title().c_str(), (int)window->Width(), (int)window->Height());
    ImGui::Text("FPS: 00.00 (will be added with Timing)");
    ImGui::Text("FPS Chart will be here also");
    auto[mX, mY] = Input::MousePos();
    ImGui::Text("Mouse Position: %i, %i", (int)mX, (int)mY);
    ImGui::End();
  }
}// namespace AGE
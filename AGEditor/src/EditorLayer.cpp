//
// Created by alex on 2/5/23.
//

#include "EditorLayer.h"

#include <Age/Renderer/RenderCommand.h>
#include <imgui.h>

namespace AGE {
  void EditorLayer::OnUpdate(Timestep ts) {
    RenderCommand::Clear();
  }

  void EditorLayer::OnUiRender(Timestep ts) {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    ImGui::Begin("Sample");
    ImGui::End();
  }
} // AGE

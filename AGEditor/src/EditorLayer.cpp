//
// Created by alex on 2/5/23.
//

#include <Age/Renderer/RenderCommand.h>
#include "EditorLayer.h"

namespace AGE {
  void EditorLayer::OnUpdate(Timestep ts) {
    RenderCommand::Clear();
  }
} // AGE

//
// Created by alex on 04.10.22.
//

#include "agepch.h"

#include "RenderCommand.h"

namespace AGE {
  RenderAPI* RenderCommand::s_RenderAPI{RenderAPI::Create()};

  void RenderCommand::Init() {
    s_RenderAPI->Init();
  }

  void RenderCommand::Clear() {
    s_RenderAPI->Clear();
  }

  void RenderCommand::SetClearColor(const glm::vec4& color) {
    s_RenderAPI->SetClearColor(color);
  }

  void RenderCommand::DrawIndexed(const Ref<VertexArray>& va) {
    s_RenderAPI->DrawIndexed(va);
  }
} // AGE
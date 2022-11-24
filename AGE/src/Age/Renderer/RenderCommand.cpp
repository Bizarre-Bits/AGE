//
// Created by alex on 04.10.22.
//

#include "agepch.h"

#include "RenderCommand.h"

namespace AGE {
  RenderAPI* RenderCommand::s_RenderAPI{RenderAPI::Create()};

  void RenderCommand::Init() {
    AGE_PROFILE_FUNCTION();

    s_RenderAPI->Init();
  }

  void RenderCommand::Clear() {
    AGE_PROFILE_FUNCTION();

    s_RenderAPI->Clear();
  }

  void RenderCommand::SetClearColor(const glm::vec4& color) {
    AGE_PROFILE_FUNCTION();

    s_RenderAPI->SetClearColor(color);
  }

  void RenderCommand::DrawIndexed(const Ref <VertexArray>& va, uint32_t count) {
    AGE_PROFILE_FUNCTION();

    s_RenderAPI->DrawIndexed(va, count);
  }
} // AGE

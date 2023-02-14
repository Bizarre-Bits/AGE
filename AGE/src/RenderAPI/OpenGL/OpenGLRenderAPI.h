//
// Created by alex on 04.10.22.
//

#pragma once

#include "Age/Renderer/RenderAPI.h"

namespace AGE {
  class OpenGLRenderAPI : public RenderAPI {
  public:
    virtual void Init() override;

    virtual void SetClearColor(const glm::vec4& color) override;
    virtual void Clear() override;

    virtual void DrawIndexed(const Ref <VertexArray>& va, uint32_t count) override;
  };
}

//
// Created by alex on 3/7/23.
//

#pragma once

#include "Age/Renderer/RenderAPI.h"

namespace AGE {
  class VulkanRenderAPI : public RenderAPI{
  public:
    void Init() override;
    void SetClearColor(const glm::vec4& color) override;
    void Clear() override;
    void DrawIndexed(const Ref <VertexArray>& va, uint32_t count) override;
  };

} // AGE

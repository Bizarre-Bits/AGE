//
// Created by alex on 04.10.22.
//

#pragma once

#include "RenderAPI.h"
#include "VertexArray.h"

namespace AGE {

  class RenderCommand {
  public:
    static void Init();

    static void Clear();
    static void SetClearColor(const glm::vec4& color);

    static void DrawIndexed(const Ref <VertexArray>& va, uint32_t count = 0);
  private:
    static RenderAPI* s_RenderAPI;
  };

} // AGE

//
// Created by alex on 04.10.22.
//

#ifndef AGE_RENDERCOMMAND_H
#define AGE_RENDERCOMMAND_H

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

#endif //AGE_RENDERCOMMAND_H

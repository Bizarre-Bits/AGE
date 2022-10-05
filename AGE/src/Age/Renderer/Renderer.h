//
// Created by alex on 04.10.22.
//

#ifndef AGE_RENDERER_H
#define AGE_RENDERER_H

#include "RenderAPI.h"
#include "VertexArray.h"
#include "glm/vec4.hpp"

namespace AGE {
  class Renderer {
  public:
    static void BeginScene();
    static void EndScene();
    static void Submit(const VertexArray* va);

    static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }
  };

} // AGE

#endif //AGE_RENDERER_H

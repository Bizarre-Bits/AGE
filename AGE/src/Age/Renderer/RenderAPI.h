//
// Created by alex on 04.10.22.
//

#ifndef AGE_RENDERAPI_H
#define AGE_RENDERAPI_H

#include "glm/vec4.hpp"
#include "VertexArray.h"

namespace AGE {

  class RenderAPI {
  public:
    enum class API {
      None,
      OpenGL,
    };
  public:
    virtual void SetClearColor(const glm::vec4& color) = 0;
    virtual void Clear() = 0;
    virtual void DrawIndexed(const Ref<VertexArray>& va) = 0;

    static API GetAPI() { return s_API; }
    static RenderAPI* Create();
  private:
    static API s_API;
  };

} // AGE


#endif //AGE_RENDERAPI_H

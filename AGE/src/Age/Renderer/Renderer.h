//
// Created by alex on 09.09.22.
//

#ifndef AGE_RENDERER_H
#define AGE_RENDERER_H

namespace AGE {
  enum class RenderAPI {
    None   = 0,
    OpenGL = 1,
  };

  class Renderer {
  public:
    static inline RenderAPI API() { return s_RenderAPI; }
    static inline void SetAPI(RenderAPI api) { s_RenderAPI = api; }
  private:
    static RenderAPI s_RenderAPI;
  };

} // AGE

#endif //AGE_RENDERER_H

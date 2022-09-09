//
// Created by alex on 09.09.22.
//

#include "Renderer.h"

namespace AGE {
  // TODO: runtime decision on RenderAPI
  RenderAPI Renderer::s_RenderAPI{RenderAPI::OpenGL};
} // AGE
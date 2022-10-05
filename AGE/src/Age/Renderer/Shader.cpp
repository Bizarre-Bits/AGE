//
// Created by alex on 04.10.22.
//

#include "agepch.h"

#include "Shader.h"
#include "Renderer.h"

#include "Age/Debug/Assert.h"

#ifdef AGE_INCLUDE_OPENGL
  #include "Platform/OpenGL/OpenGLShader.h"
#endif

namespace AGE {
  Shader* Shader::Create(const age_string_t& vertexSrc, const age_string_t& fragmentSrc) {
    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "Render API None is not supported yet");
      case RenderAPI::API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return new OpenGLShader(vertexSrc, fragmentSrc);
#else
        AGE_CORE_ASSERT(false, "OpenGL Render API is not included in the current compilation.");
#endif
    };
  }
}
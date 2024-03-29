//
// Created by alex on 11.10.22.
//

#include "agepch.h"

#include "Texture.h"
#include "Renderer.h"

#ifdef AGE_INCLUDE_OPENGL
  #include "RenderAPI/OpenGL/OpenGLTexture.h"
#else
#endif

namespace AGE {
  Ref <Texture2D> Texture2D::Create(const age_string_t& path) {
    AGE_PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return std::make_shared<OpenGLTexture2D>(path);
#else
        AGE_CORE_ASSER(false, "OpenGL is not available, because it is not included into the current compilation")
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a Vertex Array, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }

  Ref <Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height) {
    AGE_PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return std::make_shared<OpenGLTexture2D>(width, height);
#else
        AGE_CORE_ASSERT(false, "OpenGL is not available, because it is not included into the current compilation")
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a Vertex Array, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }
  Ref <Texture2D> Texture2D::ErrorTexture() {
    AGE_PROFILE_FUNCTION();

    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "RendererAPI::None is not supported");
      case RenderAPI::API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return OpenGLTexture2D::ErrorTextureImpl();
#else
        AGE_CORE_ASSERT(false, "OpenGL is not available, because it is not included into the current compilation")
#endif
      default: {
        AGE_CORE_ASSERT(false,
                        "Could not create a Vertex Array, as there is no RendererAPI selected");
      }
    }

    return nullptr;
  }
}

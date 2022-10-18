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
  Ref<Shader> Shader::Create(
      const age_string_t& name, const age_string_t& vertexSrc, const age_string_t& fragmentSrc
  ) {
    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "Render API None is not supported yet");
      case RenderAPI::API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return Ref<Shader>(new OpenGLShader(name, vertexSrc, fragmentSrc));
#else
        AGE_CORE_ASSERT(false, "OpenGL Render API is not included in the current compilation.");
#endif
    };
  }

  Ref<Shader> Shader::Create(const age_string_t& filepath) {
    switch (Renderer::GetAPI()) {
      case RenderAPI::API::None: AGE_CORE_ASSERT(false, "Render API None is not supported yet");
      case RenderAPI::API::OpenGL:
#ifdef AGE_INCLUDE_OPENGL
        return Ref<Shader>(new OpenGLShader(filepath));
#else
        AGE_CORE_ASSERT(false, "OpenGL Render API is not included in the current compilation.");
#endif
    };
  }

  // ShaderLibrary

  void ShaderLibrary::Add(const Ref<Shader>& shader) {
    const age_string_t& name = shader->GetName();
    AGE_CORE_ASSERT_FORMATTED(m_Shaders.find(name) == m_Shaders.end(),
                              "Shader '{0}' already exists!", name);
    m_Shaders[name] = shader;
  }

  Ref<Shader> ShaderLibrary::Load(const age_string_t& filepath) {
    Ref<Shader> shader = Shader::Create(filepath);
    Add(shader);
    return shader;
  }

  Ref<Shader> ShaderLibrary::Load(const age_string_t& name, const age_string_t& filepath) {
    Ref<Shader> shader = Load(filepath);
    shader->SetName(name);
    Add(shader);
    return shader;
  }

  Ref<Shader> ShaderLibrary::Get(const age_string_t& name) {
    AGE_CORE_ASSERT_FORMATTED(m_Shaders.find(name) != m_Shaders.end(),
                              "Shader '{0}' does not exist", name);
    return m_Shaders[name];
  }
}
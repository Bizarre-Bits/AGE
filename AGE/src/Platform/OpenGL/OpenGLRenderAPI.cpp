//
// Created by alex on 04.10.22.
//

#include "agepch.h"

#include "OpenGLPlatform.h"
#include "OpenGLRenderAPI.h"

namespace AGE {
  void OpenGLRenderAPI::Init() {
    AGE_PROFILE_FUNCTION();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
  }

  void OpenGLRenderAPI::SetClearColor(const glm::vec4& color) {
    AGE_PROFILE_FUNCTION();

    glClearColor(color.r, color.g, color.b, color.a);
  }

  void OpenGLRenderAPI::Clear() {
    AGE_PROFILE_FUNCTION();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void OpenGLRenderAPI::DrawIndexed(const Ref <VertexArray>& va) {
    AGE_PROFILE_FUNCTION();

    va->Bind();
    glDrawElements(GL_TRIANGLES, va->IndexBuffer()->Count(), GL_UNSIGNED_INT, nullptr);
  }
}
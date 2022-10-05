//
// Created by alex on 04.10.22.
//

#include "agepch.h"

#include "OpenGLPlatform.h"
#include "OpenGLRenderAPI.h"

namespace AGE {
  void OpenGLRenderAPI::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
  }

  void OpenGLRenderAPI::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void OpenGLRenderAPI::DrawIndexed(const AGE::VertexArray* va) {
    va->Bind();
    glDrawElements(GL_TRIANGLES, va->IndexBuffer()->Count(), GL_UNSIGNED_INT, nullptr);
  }
}
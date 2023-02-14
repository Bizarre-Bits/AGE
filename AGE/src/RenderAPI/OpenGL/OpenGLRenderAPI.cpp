//
// Created by alex on 04.10.22.
//

#include "OpenGLMaster.h"
#include "agepch.h"

#include "OpenGLRenderAPI.h"

namespace AGE {
  void OpenGLRenderAPI::Init() {
    AGE_PROFILE_FUNCTION();

    glEnable(GL_BLEND);
    glEnable(GL_ALPHA_TEST);
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

  void OpenGLRenderAPI::DrawIndexed(const Ref <VertexArray>& va, uint32_t count) {
    AGE_PROFILE_FUNCTION();

    va->Bind();
    uint32_t indexCount = count != 0 ? count : va->IndexBuffer()->Count();
    glDrawElements(GL_TRIANGLES, (int)indexCount, GL_UNSIGNED_INT, nullptr);
  }
}

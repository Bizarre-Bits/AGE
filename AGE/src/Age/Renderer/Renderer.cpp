//
// Created by alex on 04.10.22.
//
#include "agepch.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace AGE {
  Renderer::SceneData* Renderer::s_SceneData{new Renderer::SceneData};

  void Renderer::Init() {
    AGE_PROFILE_FUNCTION();

    RenderCommand::Init();
  }

  void Renderer::BeginScene(OrthographicCamera& camera) {
    AGE_PROFILE_FUNCTION();

    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::EndScene() {
    AGE_PROFILE_FUNCTION();
  }

  void Renderer::Submit(
      const Ref<VertexArray>& va, const Ref<Shader>& shader, const glm::mat4& transform
  ) {
    AGE_PROFILE_FUNCTION();

    shader->Bind();
    shader->SetMat4(
        "u_ViewProjection", s_SceneData->ViewProjectionMatrix
    );
    shader->SetMat4(
        "u_Transform", transform
    );
    RenderCommand::DrawIndexed(va, 0);
  }


} // AGE

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
    RenderCommand::Init();
  }

  void Renderer::BeginScene(OrthographicCamera& camera) {
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::EndScene() {

  }

  void Renderer::Submit(
      const Ref<VertexArray>& va, const Ref<Shader>& shader, const glm::mat4& transform
  ) {
    shader->Bind();
    shader->SetMat4(
        "u_ViewProjection", s_SceneData->ViewProjectionMatrix
    );
    shader->SetMat4(
        "u_Transform", transform
    );
    RenderCommand::DrawIndexed(va);
  }


} // AGE
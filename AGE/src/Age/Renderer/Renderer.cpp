//
// Created by alex on 04.10.22.
//
#include "agepch.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"

namespace AGE {
  Renderer::SceneData* Renderer::s_SceneData{new Renderer::SceneData};

  void Renderer::BeginScene(OrthographicCamera& camera) {
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::EndScene() {

  }
  void Renderer::Submit(const VertexArray* va, const Shader* shader) {
    shader->Bind();
    shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
    RenderCommand::DrawIndexed(va);
  }


} // AGE
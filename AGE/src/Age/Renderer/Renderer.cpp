//
// Created by alex on 04.10.22.
//
#include "agepch.h"

#include "Renderer.h"
#include "RenderCommand.h"
#include "Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace AGE {
  Renderer::SceneData* Renderer::s_SceneData{new Renderer::SceneData};

  void Renderer::BeginScene(OrthographicCamera& camera) {
    s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
  }

  void Renderer::EndScene() {

  }
  void Renderer::Submit(
      const VertexArray* va, const Shader* shader, const glm::mat4& transform
  ) {
    dynamic_cast<const OpenGLShader*>(shader)->Bind();
    dynamic_cast<const OpenGLShader*>(shader)->UploadUniformMat4(
        "u_ViewProjection", s_SceneData->ViewProjectionMatrix
    );
    dynamic_cast<const OpenGLShader*>(shader)->UploadUniformMat4(
        "u_Transform", transform
    );
    RenderCommand::DrawIndexed(va);
  }


} // AGE
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
    std::dynamic_pointer_cast<const OpenGLShader>(shader)->Bind();
    std::dynamic_pointer_cast<const OpenGLShader>(shader)->UploadUniformMat4(
        "u_ViewProjection", s_SceneData->ViewProjectionMatrix
    );
    std::dynamic_pointer_cast<const OpenGLShader>(shader)->UploadUniformMat4(
        "u_Transform", transform
    );
    RenderCommand::DrawIndexed(va);
  }


} // AGE
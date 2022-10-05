//
// Created by alex on 04.10.22.
//
#include "agepch.h"

#include "Renderer.h"
#include "RenderCommand.h"

namespace AGE {

  void Renderer::BeginScene() {

  }

  void Renderer::EndScene() {

  }

  void Renderer::Submit(const VertexArray* va) {
    RenderCommand::DrawIndexed(va);
  }
} // AGE
//
// Created by alex on 09.09.22.
//

#pragma once

#include <vector>

#include "Age/Renderer/VertexArray.h"
#include "RenderAPI/OpenGL/OpenGLBuffer.h"

namespace AGE {
  class OpenGLVertexArray : public VertexArray {
  public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) override;
    virtual void SetIndexBuffer(Ref<AGE::IndexBuffer> indexBuffer) override;

    virtual std::vector<Ref<VertexBuffer>> VertexBuffers() const override;
    virtual Ref<AGE::IndexBuffer> IndexBuffer() const override;

  private:
    std::vector<Ref<VertexBuffer>> m_VertexBuffers;

    Ref<AGE::IndexBuffer> m_IndexBuffer;

    uint32_t m_RenderID;
  };
} // AGE

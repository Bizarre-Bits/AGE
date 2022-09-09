//
// Created by alex on 09.09.22.
//

#ifndef AGE_OPENGLVERTEXARRAY_H
#define AGE_OPENGLVERTEXARRAY_H

#include <vector>

#include "Age/Renderer/VertexArray.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace AGE {
  class OpenGLVertexArray : public VertexArray {
  public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void AddVertexBuffer(const VertexBuffer* const vertexBuffer) override;
    virtual void SetIndexBuffer(const IndexBuffer* const indexBuffer) override;

  private:
    std::vector<OpenGLVertexBuffer*> m_VertexBuffers;
    OpenGLIndexBuffer* m_IndexBuffer;
    uint32_t m_RenderID;
  };
} // AGE

#endif //AGE_OPENGLVERTEXARRAY_H

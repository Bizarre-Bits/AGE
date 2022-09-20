//
// Created by alex on 09.09.22.
//

#ifndef AGE_VERTEXARRAY_H
#define AGE_VERTEXARRAY_H

#include "Age/Renderer/Buffer.h"

namespace AGE {
  class VertexArray {
  public:
    virtual ~VertexArray() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(const VertexBuffer* const vertexBuffer) = 0;
    virtual void SetIndexBuffer(const IndexBuffer* const indexBuffer) = 0;

    virtual std::vector<VertexBuffer*> VertexBuffers() const = 0;
    virtual IndexBuffer* IndexBuffer() const = 0;

    static VertexArray* Create();
  };
} // AGE

#endif //AGE_VERTEXARRAY_H

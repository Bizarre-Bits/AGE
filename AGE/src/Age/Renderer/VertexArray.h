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

    virtual void Bind() const   = 0;
    virtual void Unbind() const = 0;

    virtual void AddVertexBuffer(Ref<VertexBuffer> vertexBuffer) = 0;
    virtual void SetIndexBuffer(Ref<AGE::IndexBuffer> indexBuffer)    = 0;

    virtual std::vector<Ref<VertexBuffer>> VertexBuffers() const = 0;
    virtual Ref<AGE::IndexBuffer> IndexBuffer() const    = 0;

    static Ref<VertexArray> Create();
  };
}// namespace AGE

#endif//AGE_VERTEXARRAY_H

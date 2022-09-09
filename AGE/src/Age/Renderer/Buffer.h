//
// Created by alex on 09.09.22.
//

#ifndef AGE_VERTEXBUFFER_H
#define AGE_VERTEXBUFFER_H

#include <cinttypes>

#include "Renderer.h"

namespace AGE {
  class VertexBuffer {
  public:
    virtual ~VertexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t Count() const = 0;

    static VertexBuffer* Create(float* vertices, uint32_t size);
  };

  class IndexBuffer {
  public:
    virtual ~IndexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t Count() const = 0;

    static IndexBuffer* Create(uint32_t* indices, uint32_t size);
  };
}

#endif //AGE_VERTEXBUFFER_H

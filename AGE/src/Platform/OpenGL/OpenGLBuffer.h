//
// Created by alex on 09.09.22.
//

#ifndef AGE_OPENGLBUFFER_H
#define AGE_OPENGLBUFFER_H

#include "Platform/OpenGL/OpenGLPlatform.h"

#include "Age/Renderer/Buffer.h"

#include <cinttypes>

namespace AGE {

  GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);

  class OpenGLVertexBuffer : public VertexBuffer {
  public:
    OpenGLVertexBuffer(float* vertices, uint32_t count);
    virtual ~OpenGLVertexBuffer() override;

    virtual inline void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };
    virtual inline const BufferLayout& Layout() const override { return m_Layout; };

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual uint32_t Count() const override;

  private:
    uint32_t m_RendererID;
    uint32_t m_Count;

    BufferLayout m_Layout;
  };

  class OpenGLIndexBuffer : public IndexBuffer {
  public:
    OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
    virtual ~OpenGLIndexBuffer() override;

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual uint32_t Count() const override;

  private:
    uint32_t m_RendererID;
    uint32_t m_Count;
  };

} // AGE

#endif //AGE_OPENGLBUFFER_H

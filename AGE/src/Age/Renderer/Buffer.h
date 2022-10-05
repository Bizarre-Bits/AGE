//
// Created by alex on 09.09.22.
//

#ifndef AGE_VERTEXBUFFER_H
#define AGE_VERTEXBUFFER_H

#include <vector>

#include <cinttypes>

#include "Age/Core/Core.h"

namespace AGE {

  enum class ShaderDataType : uint8_t {
    // xxx'00000 - Type
    // 000'xxxxx - Component Count
    // 000 - Float
    // 001 - Int
    // 010 - Bool
    None = 0,

    Float  = 0b000'00001,
    Float2 = 0b000'00010,
    Float3 = 0b000'00011,
    Float4 = 0b000'00100,

    FloatMat3 = 0b000'01001,
    FloatMat4 = 0b000'10000,

    Int  = 0b001'00001,
    Int2 = 0b001'00010,
    Int3 = 0b001'00011,
    Int4 = 0b001'00100,

    Bool = 0b010'00001
  };

  uint32_t shader_data_size(ShaderDataType type);

  /**
   * @brief Converts ShaderDataType objects to corresponding ShaderDataType but with only one component.
   * For example, Float4 will be converted to Float.
   * @param type - type to be converted
   * @return The same ShaderDataType but with one component.
   */
  ShaderDataType base_shader_data_type(ShaderDataType type);

  struct BufferElement {
    age_string_t   Name;
    ShaderDataType Type;
    uint32_t       Size;
    uint32_t       Offset;
    bool           Normalized;

    BufferElement(const age_string_t& name, ShaderDataType type, bool normalized = false)
        : Name{name}, Type{type}, Size{shader_data_size(type)}, Offset{0}, Normalized{normalized} {}

    uint32_t ComponentCount() const;
  };

  class BufferLayout {
  public:
    BufferLayout(const std::initializer_list<BufferElement>& elements);
    BufferLayout() = default;

    inline const std::vector<BufferElement>& Elements() const { return m_Elements; }
    inline const uint32_t Stride() const { return m_Stride; }

    inline auto begin() const { return m_Elements.begin(); }
    inline auto end() const { return m_Elements.end(); }

  private:
    void calculateOffsetsAndStride();

    std::vector<BufferElement> m_Elements;
    uint32_t                   m_Stride;
  };


  class VertexBuffer {
  public:
    virtual ~VertexBuffer() {}

    virtual void SetLayout(const BufferLayout& layout) = 0;
    virtual const BufferLayout& Layout() const = 0;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t Count() const = 0;

    static VertexBuffer* Create(float* vertices, uint32_t count);
  };

  class IndexBuffer {
  public:
    virtual ~IndexBuffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t Count() const = 0;

    static IndexBuffer* Create(uint32_t* indices, uint32_t count);
  };
}

#endif //AGE_VERTEXBUFFER_H

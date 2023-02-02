//
// Created by alex on 11.10.22.
//

#ifndef AGE_TEXTURE_H
#define AGE_TEXTURE_H

#include "Age/Core/Core.h"

namespace AGE {
  class Texture {
  public:
    virtual ~Texture() = default;

    virtual void SetData(void* data, uint32_t size) = 0;

    virtual uint32_t Width() const = 0;
    virtual uint32_t Height() const = 0;
    virtual uint32_t ID() const = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t Slot() const = 0;
    virtual bool IsCorrect() const = 0;
  };

  class Texture2D : public Texture {
  public:
    static Ref<Texture2D> Create(const age_string_t& path);
    static Ref<Texture2D> Create(const uint32_t width, const uint32_t height);
    static Ref<Texture2D> ErrorTexture();

    virtual bool operator==(const Texture2D& other) const { return ID() == other.ID(); };
  };
}

#endif //AGE_TEXTURE_H

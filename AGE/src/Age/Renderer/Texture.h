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

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;
  };

  class Texture2D : public Texture {
  public:
    static Ref<Texture2D> Create(const age_string_t& path);
  };
}

#endif //AGE_TEXTURE_H

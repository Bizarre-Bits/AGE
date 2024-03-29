//
// Created by alex on 08.09.22.
//

#pragma once

namespace AGE {
  class Context {
  public:
    Context() = default;
    virtual ~Context() = default;

    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;
  };

}// namespace AGE

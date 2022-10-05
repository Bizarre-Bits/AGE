//
// Created by alex on 08.09.22.
//

#ifndef AGE_CONTEXT_H
#define AGE_CONTEXT_H

namespace AGE {
  class Context {
  public:
    Context() = default;
    virtual ~Context() = default;

    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;
  };

}// namespace AGE

#endif//AGE_CONTEXT_H

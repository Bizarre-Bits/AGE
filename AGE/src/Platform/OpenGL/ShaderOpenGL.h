//
// Created by alex on 08.09.22.
//

#ifndef AGE_SHADEROPENGL_H
#define AGE_SHADEROPENGL_H

#include "Age/Core/Core.h"
#include "Age/Renderer/Shader.h"

namespace AGE {
  class ShaderOpenGL : public Shader {
  public:
    ShaderOpenGL(const age_string_t& vertexSrc, const age_string_t& fragmentSrc);
    ~ShaderOpenGL();

    virtual void Bind() const override;
    virtual void Unbind() const override;

  private:
    unsigned int m_RendererID;
  };
}

#endif //AGE_SHADEROPENGL_H

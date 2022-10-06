//
// Created by alex on 08.09.22.
//

#ifndef AGE_OPENGLSHADER_H
#define AGE_OPENGLSHADER_H

#include "Age/Core/Core.h"
#include "Age/Renderer/Shader.h"

namespace AGE {
  class OpenGLShader : public Shader {
  public:
    OpenGLShader(const age_string_t& vertexSrc, const age_string_t& fragmentSrc);
    ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void UploadUniformMat4(const age_string_t& name, const glm::mat4& matrix) const override;

  private:
    unsigned int m_RendererID;
  };
}

#endif //AGE_OPENGLSHADER_H

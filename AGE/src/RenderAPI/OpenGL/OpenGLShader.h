//
// Created by alex on 08.09.22.
//

#pragma once

#include <unordered_map>

#include "Age/Core/Core.h"
#include "Age/Renderer/Shader.h"
#include "OpenGLMaster.h"

namespace AGE {
  class OpenGLShader : public Shader {
  public:
    OpenGLShader(
        const age_string_t& name, const age_string_t& vertexSrc, const age_string_t& fragmentSrc
    );
    OpenGLShader(const age_string_t filepath);
    ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetMat4(const age_string_t& name, const glm::mat4& value) override;

    virtual void SetFloat4(const age_string_t& name, const glm::vec4& value) override;
    virtual void SetFloat3(const age_string_t& name, const glm::vec3& value) override;
    virtual void SetFloat2(const age_string_t& name, const glm::vec2& value) override;
    virtual void SetFloat(const age_string_t& name, const float value) override;

    virtual void SetInt(const age_string_t& name, const int value) override;
    virtual void SetIntArray(const age_string_t& name, const int* values, uint32_t count) override;

    virtual inline const age_string_t& GetName() const override { return m_Name; }

  protected:
    virtual void SetName(const age_string_t& value) override { m_Name = value; }

  private:
    age_string_t LoadShaderFile(const age_string_t& filepath) const;
    GLuint StringToShaderType(const age_string_t& source) const;
    std::unordered_map<GLenum, age_string_t> MapShaders(const age_string_t& shaderSrc);
    void Compile(const std::unordered_map<GLenum, age_string_t>& shaderSrcs);

    void UploadUniformFloat(const age_string_t& name, const float value) const;
    void UploadUniformFloat2(const age_string_t& name, const glm::vec2& vector) const;
    void UploadUniformFloat3(const age_string_t& name, const glm::vec3& vector) const;
    void UploadUniformFloat4(const age_string_t& name, const glm::vec4& vector) const;

    void UploadUniformMat4(const age_string_t& name, const glm::mat4& matrix) const;
    void UploadUniformMat3(const age_string_t& name, const glm::mat3& matrix) const;

    void UploadUniformInt(const age_string_t& name, const int value) const;

  private:
    unsigned int m_RendererID;
    age_string_t m_Name;
  };
}

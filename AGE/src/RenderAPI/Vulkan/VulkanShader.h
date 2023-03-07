//
// Created by alex on 3/7/23.
//

#pragma once

#include "Age/Renderer/Shader.h"

namespace AGE {
  class VulkanShader : public Shader {
  public:
    VulkanShader(const age_string_t& name, const age_string_t& vertexSrc, const age_string_t& fragmentSrc);
    VulkanShader(const age_string_t& filepath);
    ~VulkanShader() override = default;
    void Bind() const override;
    void Unbind() const override;
    void SetMat4(const age_string_t& name, const glm::mat4& value) override;
    void SetFloat4(const age_string_t& name, const glm::vec4& value) override;
    void SetFloat3(const age_string_t& name, const glm::vec3& value) override;
    void SetFloat2(const age_string_t& name, const glm::vec2& value) override;
    void SetFloat(const age_string_t& name, const float value) override;
    void SetInt(const age_string_t& name, const int value) override;
    void SetIntArray(const age_string_t& name, const int* values, uint32_t count) override;
    const age_string_t& GetName() const override;
  protected:
    void SetName(const age_string_t& value) override;

  private:
    age_string_t m_Name{"Not implemented shader"};
  };
} // AGE

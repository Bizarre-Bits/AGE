//
// Created by alex on 3/7/23.
//


#include "agepch.h"

#include "VulkanShader.h"

namespace AGE {
  VulkanShader::VulkanShader(const age_string_t& name, const age_string_t& vertexSrc, const age_string_t& fragmentSrc) {

  }

  VulkanShader::VulkanShader(const age_string_t& filepath) {

  }

  void VulkanShader::Bind() const {

  }

  void VulkanShader::Unbind() const {

  }

  void VulkanShader::SetMat4(const age_string_t& name, const glm::mat4& value) {

  }

  void VulkanShader::SetFloat4(const age_string_t& name, const glm::vec4& value) {

  }

  void VulkanShader::SetFloat3(const age_string_t& name, const glm::vec3& value) {

  }

  void VulkanShader::SetFloat2(const age_string_t& name, const glm::vec2& value) {

  }

  void VulkanShader::SetFloat(const age_string_t& name, const float value) {

  }

  void VulkanShader::SetInt(const age_string_t& name, const int value) {

  }

  void VulkanShader::SetIntArray(const age_string_t& name, const int* values, uint32_t count) {

  }

  const age_string_t& VulkanShader::GetName() const {
    return m_Name;
  }

  void VulkanShader::SetName(const age_string_t& value) {

  }
} // AGE
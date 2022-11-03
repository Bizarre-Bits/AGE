//
// Created by alex on 08.09.22.
//

#ifndef AGE_SHADER_H
#define AGE_SHADER_H

#include <glm/glm.hpp>
#include <unordered_map>

#include "Age/Core/Core.h"

namespace AGE {

  class Shader {
  public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetMat4(const age_string_t& name, const glm::mat4& value) = 0;
    virtual void SetFloat4(const age_string_t& name, const glm::vec4& value) = 0;
    virtual void SetFloat3(const age_string_t& name, const glm::vec3& value) = 0;
    virtual void SetFloat2(const age_string_t& name, const glm::vec2& value) = 0;
    virtual void SetFloat(const age_string_t& name, const float value) = 0;
    virtual void SetInt(const age_string_t& name, const int value) = 0;

    virtual const age_string_t& GetName() const = 0;

    static Ref<Shader> Create(
        const age_string_t& name, const age_string_t& vertexSrc, const age_string_t& fragmentSrc
    );
    static Ref<Shader> Create(const age_string_t& filepath);

    friend class ShaderLibrary;

  protected:
    virtual void SetName(const age_string_t& value) = 0;

    friend class ShaderLibrary;
  };

  class ShaderLibrary {
  public:
    void Add(const Ref<Shader>& shader);

    Ref<Shader> Load(const age_string_t& filepath);
    Ref<Shader> Load(const age_string_t& name, const age_string_t& filepath);
    Ref<Shader> Get(const age_string_t& name);
  private:
    std::unordered_map<age_string_t, Ref<Shader>> m_Shaders;
  };

} // AGE

#endif //AGE_SHADER_H

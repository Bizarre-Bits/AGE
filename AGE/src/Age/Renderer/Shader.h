//
// Created by alex on 08.09.22.
//

#ifndef AGE_SHADER_H
#define AGE_SHADER_H

#include "Age/Core/Core.h"

#include <glm/glm.hpp>

namespace AGE {

  class Shader {
  public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

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
    void Add(const Ref <Shader>& shader);

    Ref <Shader> Load(const age_string_t& filepath);
    Ref <Shader> Load(const age_string_t& name, const age_string_t& filepath);
    Ref <Shader> Get(const age_string_t& name);
  private:
    std::unordered_map<age_string_t, Ref < Shader>> m_Shaders;
  };

} // AGE

#endif //AGE_SHADER_H

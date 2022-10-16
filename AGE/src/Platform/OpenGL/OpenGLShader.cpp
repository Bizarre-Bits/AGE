//
// Created by alex on 08.09.22.
//
#include "agepch.h"

#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLPlatform.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace AGE {
  OpenGLShader::OpenGLShader(
      const age_string_t& vertexSrc, const age_string_t& fragmentSrc
  ) {
    std::unordered_map<GLenum, age_string_t> sources{std::pair{GL_VERTEX_SHADER, vertexSrc},
                                                     std::pair{GL_FRAGMENT_SHADER, fragmentSrc}};
    Compile(sources);
  }
  OpenGLShader::OpenGLShader(const age_string_t filepath) {

  }

  OpenGLShader::~OpenGLShader() {

  }

  void OpenGLShader::Compile(const std::unordered_map<GLenum, age_string_t>& sources) {
    GLuint shaderIDS[sources.size()];
    GLuint* nextShaderInsert{shaderIDS};

    m_RendererID = glCreateProgram();

    for (auto source: sources) {
      auto [shaderType, sourceStr] = source;

      GLuint shaderID = glCreateShader(shaderType);

      auto shaderCSrc = (GLchar*)sourceStr.c_str();
      glShaderSource(shaderID, 1, &shaderCSrc, 0);
      glCompileShader(shaderID);

      GLint isCompiled{0};
      glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
      if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
        char infoLog[maxLength];
        glGetShaderInfoLog(shaderID, maxLength, &maxLength, infoLog);

        glDeleteShader(shaderID);
        AGE_CORE_ERROR("Failed to compile shader: \n {0}", age_string_t{infoLog});
        continue;
      }

      glAttachShader(m_RendererID, shaderID);

      *nextShaderInsert = shaderID;
      nextShaderInsert++;
    }

    glLinkProgram(m_RendererID);

    GLint isLinked{0};
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);
      char infoLog[maxLength];
      glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, infoLog);

      glDeleteProgram(m_RendererID);
      AGE_CORE_ERROR("Failed to link program: \n {0}", age_string_t{infoLog});
    }

    for (auto shader: shaderIDS) {
      glDetachShader(m_RendererID, shader);
      glDeleteShader(shader);
    }

  }

  void OpenGLShader::Bind() const {
    glUseProgram(m_RendererID);
  }

  void OpenGLShader::Unbind() const {
    glUseProgram(0);
  }

  void OpenGLShader::UploadUniformFloat(const age_string_t& name, const float value) const {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
  }

  void OpenGLShader::UploadUniformFloat2(const age_string_t& name, const glm::vec2& vector) const {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, vector.x, vector.y);
  }

  void OpenGLShader::UploadUniformFloat3(const age_string_t& name, const glm::vec3& vector) const {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, vector.x, vector.y, vector.z);
  }
  void OpenGLShader::UploadUniformFloat4(const age_string_t& name, const glm::vec4& vector) const {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
  }

  void OpenGLShader::UploadUniformMat3(const age_string_t& name, const glm::mat3& matrix) const {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void OpenGLShader::UploadUniformMat4(const age_string_t& name, const glm::mat4& matrix) const {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void OpenGLShader::UploadUniformInt(const age_string_t& name, const int value) const {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
  }

}

//
// Created by alex on 08.09.22.
//
#include "agepch.h"

#include <unordered_map>
#include <fstream>

#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLPlatform.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Age/Debug/Assert.h"

namespace AGE {
  OpenGLShader::OpenGLShader(
      const age_string_t& vertexSrc, const age_string_t& fragmentSrc
  ) {
    std::unordered_map<GLenum, age_string_t> sources{std::pair{GL_VERTEX_SHADER, vertexSrc},
                                                     std::pair{GL_FRAGMENT_SHADER, fragmentSrc}};
    Compile(sources);
  }
  OpenGLShader::OpenGLShader(const age_string_t filepath) {
    const age_string_t shader{LoadShaderFile(filepath)};
    Compile(MapShaders(shader));
  }

  OpenGLShader::~OpenGLShader() {

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

  age_string_t OpenGLShader::LoadShaderFile(const age_string_t& filepath) const {
    age_string_t  result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in) {
      in.seekg(0, std::ios::end);
      size_t size = in.tellg();
      if (size != -1) {
        result.resize(size);
        in.seekg(0, std::ios::beg);
        in.read(&result[0], size);
      } else {
        AGE_CORE_ERROR("Could not read from file '{0}'", filepath);
      }
    } else {
      AGE_CORE_ERROR("Could not open file '{0}'", filepath);
    }
    return result;
  }

  std::unordered_map<GLenum, age_string_t> OpenGLShader::MapShaders(const age_string_t& shaderSrc) {
    std::unordered_map<GLenum, age_string_t> shaderMap;

    const char* typeToken{"#type"};
    size_t typeTokenSize{strlen(typeToken)};

    size_t pos = shaderSrc.find(typeToken, 0);
    while (pos != age_string_t::npos) {
      size_t eol = shaderSrc.find_first_of("\r\n", pos);
      AGE_CORE_ASSERT(eol != age_string_t::npos, "Syntax error");

      size_t       begin = pos + typeTokenSize + 1;
      age_string_t type{shaderSrc.substr(begin, eol - begin)};

      size_t nextLinePos = shaderSrc.find_first_not_of("\r\n", eol);
      AGE_CORE_ASSERT(nextLinePos != age_string_t::npos, "Syntax error");

      pos = shaderSrc.find(typeToken, nextLinePos);

      shaderMap[StringToShaderType(type)] = (pos == age_string_t::npos) ? shaderSrc.substr(
          nextLinePos
      ) : shaderSrc.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderMap;
  }

  GLuint OpenGLShader::StringToShaderType(const age_string_t& source) const {
    if (source == "vertex")
      return GL_VERTEX_SHADER;
    if (source == "fragment")
      return GL_FRAGMENT_SHADER;
    if (source == "geometry")
      return GL_GEOMETRY_SHADER;

    AGE_CORE_ERROR("Unknown shader type: '{0}'", source);
    return 0;
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

}

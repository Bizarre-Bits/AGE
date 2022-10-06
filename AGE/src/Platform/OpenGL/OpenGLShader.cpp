//
// Created by alex on 08.09.22.
//
#include "agepch.h"

#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLPlatform.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace AGE {
  OpenGLShader::OpenGLShader(const age_string_t& vertexSrc, const age_string_t& fragmentSrc) {


    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = (const GLchar*)vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

      // Use the infoLog as you see fit.
      AGE_CORE_ERROR("Failed to compile vertex shader:\n {0}", age_string_t(infoLog.begin(), infoLog.end() - 1));
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = (const GLchar*)fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
      GLint maxLength = 0;
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

      // Use the infoLog as you see fit.
      AGE_CORE_ERROR("Failed to compile fragment shader:\n {0}", age_string_t(infoLog.begin(), infoLog.end() - 1));
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_RendererID = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(m_RendererID, vertexShader);
    glAttachShader(m_RendererID, fragmentShader);

    // Link our program
    glLinkProgram(m_RendererID);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE) {
      GLint maxLength = 0;
      glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

      // We don't need the program anymore.
      glDeleteProgram(m_RendererID);
      // Don't leak shaders either.
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      // Use the infoLog as you see fit.
      AGE_CORE_ERROR("Failed to link shader program:\n {0}", age_string_t(infoLog.begin(), infoLog.end() - 1));
    }

// Always detach shaders after a successful link.
    glDetachShader(m_RendererID, vertexShader);
    glDetachShader(m_RendererID, fragmentShader);
  }

  OpenGLShader::~OpenGLShader() {

  }

  void OpenGLShader::UploadUniformMat4(const age_string_t& name, const glm::mat4& matrix) const {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void OpenGLShader::Bind() const {
    glUseProgram(m_RendererID);
  }

  void OpenGLShader::Unbind() const {
    glUseProgram(0);
  }

}

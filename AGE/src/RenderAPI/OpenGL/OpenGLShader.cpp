//
// Created by alex on 08.09.22.
//

#include "agepch.h"
#include "OpenGLMaster.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "OpenGLShader.h"
#include "OpenGLShaderUtils.h"
#include "Age/Core/Timer.h"

namespace AGE {
  OpenGLShader::OpenGLShader(
      const age_string_t& name, const age_string_t& vertexSrc, const age_string_t& fragmentSrc
  ) : m_Name(name) {
    AGE_PROFILE_FUNCTION();

    std::unordered_map<GLenum, age_string_t> sources;
    sources[GL_VERTEX_SHADER] = vertexSrc;
    sources[GL_FRAGMENT_SHADER] = fragmentSrc;

    CompileOrGetVulkanBinaries(sources);
    CompileOrGetOpenGLBinaries();
    CreateProgram();
  }

  OpenGLShader::OpenGLShader(const age_string_t& filepath) : m_Filepath{filepath} {
    AGE_PROFILE_FUNCTION();

    ShaderUtils::CreateCacheDirectoryIfNeeded();

    age_string_t const source = ShaderUtils::LoadShaderFile(filepath);
    auto shaderSources = ShaderUtils::MapShaders(source);
    {
      Timer timer;
      timer.Start();
      CompileOrGetVulkanBinaries(shaderSources);
      CompileOrGetOpenGLBinaries();
      CreateProgram();
      AGE_CORE_INFO("Compiled shaders in {0}ms", timer.DeltaTime().Milliseconds());
    }
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = lastSlash == age_string_t::npos ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind('.');
    auto count = lastDot == age_string_t::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    m_Name = filepath.substr(lastSlash, count);
  }

  OpenGLShader::~OpenGLShader() {
    glDeleteProgram(m_RendererID);
  }

  void OpenGLShader::Bind() const {
    AGE_PROFILE_FUNCTION();

    glUseProgram(m_RendererID);
  }

  void OpenGLShader::Unbind() const {
    AGE_PROFILE_FUNCTION();

    glUseProgram(0);
  }

  void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, age_string_t>& shaderSrcs) {
    const shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
    constexpr bool optimize = true;
    if constexpr (optimize) {
      options.SetOptimizationLevel(shaderc_optimization_level_performance);
    }
    const std::filesystem::path cacheDirectory = ShaderUtils::CacheDirectory();

    auto& shaderData = m_VulkanSPIRV;
    for (auto&& [stage, source]: shaderSrcs) {
      const std::filesystem::path shaderFilepath = m_Filepath;
      const std::filesystem::path cachedFilepath =
          cacheDirectory / (shaderFilepath.filename().string() + ShaderUtils::GLShaderCachedVulkanFileExtension(stage));
      std::ifstream in(cachedFilepath, std::ios::in | std::ios::binary);

      bool isCacheOutdated{false};

      if(in.is_open()) {
        auto cacheModificationTime = std::filesystem::last_write_time(cachedFilepath);
        auto shaderModificationTime = std::filesystem::last_write_time(shaderFilepath);

        if(shaderModificationTime > cacheModificationTime)
          isCacheOutdated = true;
      }

      if (in.is_open() && !isCacheOutdated) {
        in.seekg(0, std::ios::end);
        auto size = in.tellg();
        in.seekg(std::ios::beg);

        auto& data = shaderData[stage];
        data.resize(size / sizeof(uint32_t));
        in.read((char*)data.data(), size);
      } else {
        AGE_CORE_TRACE("Recompiling shader({0}) because cache is outdated or does not exist", cachedFilepath.string());

        const shaderc::SpvCompilationResult shaderModule = compiler.CompileGlslToSpv(
            source,
            ShaderUtils::GLShaderStageToShaderC(stage),
            m_Filepath.c_str(),
            options
        );
        if (shaderModule.GetCompilationStatus() != shaderc_compilation_status_success) {
          AGE_CORE_ERROR(shaderModule.GetErrorMessage());
          AGE_CORE_ASSERT(false);
        }
        shaderData[stage] = std::vector<uint32_t>(shaderModule.cbegin(), shaderModule.cend());

        std::ofstream out(cachedFilepath, std::ios::out | std::ios::binary);
        if (out.is_open()) {
          auto& data = shaderData[stage];
          out.write((char*)data.data(), data.size() * sizeof(uint32_t));
          out.flush();
          out.close();
        }
      }
    }

    for (auto&& [stage, data]: shaderData)
      Reflect(stage, data);
  }

  void OpenGLShader::CompileOrGetOpenGLBinaries() {
    auto& shaderData = m_OpenGLSPIRV;
    const shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
    constexpr bool optimize = true;
    if constexpr (optimize) {
      options.SetOptimizationLevel(shaderc_optimization_level_performance);
    }

    const std::filesystem::path cacheDirectory = ShaderUtils::CacheDirectory();
    shaderData.clear();
    m_OpenGLSources.clear();
    for (auto&& [stage, spirv]: m_VulkanSPIRV) {
      std::filesystem::path shaderFilepath = m_Filepath;
      std::filesystem::path cachedFilepath =
          cacheDirectory / (shaderFilepath.filename().string() + ShaderUtils::GLShaderCachedOpenGLFileExtension(stage));

      std::ifstream in(cachedFilepath, std::ios::in | std::ios::binary);

      bool isCacheOutdated{false};
      if(in.is_open()) {
        auto cacheModificationTime = std::filesystem::last_write_time(cachedFilepath);
        auto shaderModificationTime = std::filesystem::last_write_time(shaderFilepath);

        if(shaderModificationTime > cacheModificationTime)
          isCacheOutdated = true;
      }

      if (in.is_open() && !isCacheOutdated) {
        in.seekg(0, std::ios::end);
        auto size = in.tellg();
        in.seekg(std::ios::beg);

        auto& data = shaderData[stage];
        data.resize(size / sizeof(uint32_t));
        in.read((char*)data.data(), size);
      } else {
        AGE_CORE_TRACE("Recompiling shader({0}) because cache is outdated or does not exist", cachedFilepath.string());

        spirv_cross::CompilerGLSL glslCompiler(spirv);
        m_OpenGLSources[stage] = glslCompiler.compile();
        auto& source = m_OpenGLSources[stage];

        shaderc::SpvCompilationResult shaderModule = compiler.CompileGlslToSpv(
            source, ShaderUtils::GLShaderStageToShaderC(stage), m_Filepath.c_str()
        );
        if (shaderModule.GetCompilationStatus() != shaderc_compilation_status_success) {
          AGE_CORE_ERROR(shaderModule.GetErrorMessage());
          AGE_CORE_ASSERT(false);
        }

        shaderData[stage] = std::vector<uint32_t>(shaderModule.cbegin(), shaderModule.cend());

        std::ofstream out(cachedFilepath, std::ios::out | std::ios::binary);
        if(out.is_open()) {
          auto& data = shaderData[stage];
          out.write((char*)data.data(), data.size() * sizeof(uint32_t));
          out.flush();
          out.close();
        }
      }
    }
  }

  void OpenGLShader::CreateProgram() {
    GLuint program = glCreateProgram();

    std::vector<GLuint> shaderIDs;
    for(auto&& [stage, spirv] : m_OpenGLSPIRV) {
      GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
      glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
      glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
      glAttachShader(program, shaderID);
    }

    glLinkProgram(program);

    GLint isLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if(isLinked == GL_FALSE) {
      GLint maxLength;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
      AGE_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_Filepath, infoLog.data());

      glDeleteProgram(program);
      for(auto id: shaderIDs)
        glDeleteShader(id);
    }

    m_RendererID = program;
  }

  void OpenGLShader::Reflect(GLenum stage, const std::vector<uint32_t>& shaderData) {
    spirv_cross::Compiler compiler(shaderData);
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    AGE_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", ShaderUtils::GLShaderStageToString(stage), m_Filepath);
    AGE_CORE_TRACE("  {0} uniform buffers", resources.uniform_buffers.size());
    AGE_CORE_TRACE("  {0} resources", resources.sampled_images.size());

    AGE_CORE_TRACE("Uniform buffers:");
    for(const auto& resource : resources.uniform_buffers) {
      const auto& bufferType = compiler.get_type(resource.base_type_id);
      uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
      uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
      int memberCount = bufferType.member_types.size();

      AGE_CORE_TRACE("  {0}", resource.name);
      AGE_CORE_TRACE("    size = {0}", bufferSize);
      AGE_CORE_TRACE("    Binding = {0}", binding);
      AGE_CORE_TRACE("    Members = {0}", memberCount);
    }
  }


#pragma region Uniforms

  void OpenGLShader::UploadUniformFloat(const age_string_t& name, const float value) const {
    AGE_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
  }

  void OpenGLShader::UploadUniformFloat2(const age_string_t& name, const glm::vec2& vector) const {
    AGE_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, vector.x, vector.y);
  }

  void OpenGLShader::UploadUniformFloat3(const age_string_t& name, const glm::vec3& vector) const {
    AGE_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, vector.x, vector.y, vector.z);
  }

  void OpenGLShader::UploadUniformFloat4(const age_string_t& name, const glm::vec4& vector) const {
    AGE_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
  }

  void OpenGLShader::UploadUniformMat3(const age_string_t& name, const glm::mat3& matrix) const {
    AGE_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void OpenGLShader::UploadUniformMat4(const age_string_t& name, const glm::mat4& matrix) const {
    AGE_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
  }

  void OpenGLShader::UploadUniformInt(const age_string_t& name, const int value) const {
    AGE_PROFILE_FUNCTION();

    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
  }

  void OpenGLShader::SetMat4(const age_string_t& name, const glm::mat4& value) {
    AGE_PROFILE_FUNCTION();

//    UploadUniformMat4(name, value);
    glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(value));
  }

  void OpenGLShader::SetFloat4(const age_string_t& name, const glm::vec4& value) {
    AGE_PROFILE_FUNCTION();

    UploadUniformFloat4(name, value);
  }

  void OpenGLShader::SetFloat3(const age_string_t& name, const glm::vec3& value) {
    AGE_PROFILE_FUNCTION();

    UploadUniformFloat3(name, value);
  }

  void OpenGLShader::SetFloat2(const age_string_t& name, const glm::vec2& value) {
    AGE_PROFILE_FUNCTION();

    UploadUniformFloat2(name, value);
  }

  void OpenGLShader::SetFloat(const age_string_t& name, const float value) {
    AGE_PROFILE_FUNCTION();

    UploadUniformFloat(name, value);
  }

  void OpenGLShader::SetInt(const age_string_t& name, const int value) {
    AGE_PROFILE_FUNCTION();

    UploadUniformInt(name, value);
  }

  void OpenGLShader::SetIntArray(const age_string_t& name, const int* values, uint32_t count) {
    AGE_PROFILE_FUNCTION();

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1iv(location, count, values);
  }

#pragma endregion

}

//
// Created by alex on 3/2/23.
//

#include "OpenGLShaderUtils.h"

namespace AGE::ShaderUtils {

  GLenum StringToShaderType(const age_string_t& source) {
    AGE_PROFILE_FUNCTION();

    if (source == "vertex")
      return GL_VERTEX_SHADER;
    if (source == "fragment")
      return GL_FRAGMENT_SHADER;
    if (source == "geometry")
      return GL_GEOMETRY_SHADER;

    AGE_CORE_ERROR("Unknown shader type: '{0}'", source);
    return 0;
  }

  shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
    switch (stage) {
      case GL_VERTEX_SHADER:
        return shaderc_glsl_vertex_shader;
      case GL_FRAGMENT_SHADER:
        return shaderc_glsl_fragment_shader;
      default:
        break;
    }
    AGE_CORE_ASSERT(false, "Unknown shader stage");
    return (shaderc_shader_kind)0;
  }

  const char* GLShaderStageToString(GLenum stage) {
    switch (stage) {
      case GL_VERTEX_SHADER:
        return "vertex";
      case GL_FRAGMENT_SHADER:
        return "fragment";
      default:
        break;
    }
    AGE_CORE_ASSERT(false, "Unknown shader stage");
    return "";
  }

  const char* CacheDirectory() {
    return "assets/cache/shader/opengl";
  }

  void CreateCacheDirectoryIfNeeded() {
    age_string_t cacheDirectory = CacheDirectory();
    if (!std::filesystem::exists(cacheDirectory))
      std::filesystem::create_directories(cacheDirectory);
  }

  age_string_t LoadShaderFile(const age_string_t& filepath) {

    AGE_PROFILE_FUNCTION();

    age_string_t  result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in) {
      in.seekg(0, std::ios::end);
      size_t size = in.tellg();
      if (size != -1) {
        result.resize(size);
        in.seekg(0, std::ios::beg);
        in.read(&result[0], (int32_t)size);
      } else {
        AGE_CORE_ERROR("Could not read from file '{0}'", filepath);
      }
    } else {
      AGE_CORE_ERROR("Could not open file '{0}'", filepath);
    }
    return result;
  }

  std::unordered_map<GLenum, age_string_t> MapShaders(const age_string_t& shaderSrc) {
    AGE_PROFILE_FUNCTION();

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

  const char* GLShaderCachedOpenGLFileExtension(GLenum stage) {
    switch (stage) {
      case GL_VERTEX_SHADER:
        return ".cached_opengl.vert";
      case GL_FRAGMENT_SHADER:
        return ".cached_opengl.frag";
      default:
        break;
    }
    AGE_CORE_ASSERT(false);
    return "";
  }

  const char* GLShaderCachedVulkanFileExtension(GLenum stage) {
    switch (stage) {
      case GL_VERTEX_SHADER:
        return ".cached_vulkan.vert";
      case GL_FRAGMENT_SHADER:
        return ".cached_vulkan.frag";
      default:
        break;
    }
    AGE_CORE_ASSERT(false);
    return "";
  }
}

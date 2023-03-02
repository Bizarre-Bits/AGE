//
// Created by alex on 3/2/23.
//

#pragma once

#include <shaderc/shaderc.hpp>

#include "OpenGLMaster.h"
#include "Age/Core/Core.h"

namespace AGE::ShaderUtils {
  GLenum StringToShaderType(const age_string_t& source);
  shaderc_shader_kind GLShaderStageToShaderC(GLenum stage);
  const char* GLShaderStageToString(GLenum stage);
  const char* CacheDirectory();
  void CreateCacheDirectoryIfNeeded();
  const char* GLShaderCachedOpenGLFileExtension(GLenum stage);
  const char* GLShaderCachedVulkanFileExtension(GLenum stage);

  age_string_t LoadShaderFile(const age_string_t& filepath);
  std::unordered_map<GLenum, age_string_t> MapShaders(const age_string_t& shaderSrc);
}

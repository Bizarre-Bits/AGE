//
// Created by a on 24.08.22.
//

#pragma once

#include "Age/Core/Core.h"

//#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

namespace AGE {

  class  Log {
  public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& CoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& ClientLogger() { return s_ClientLogger; }

  private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
  };

}// namespace AGE

#ifdef DEBUG
  //Core log macros
  #define AGE_CORE_TRACE(...) ::AGE::Log::CoreLogger()->trace(__VA_ARGS__)
  #define AGE_CORE_INFO(...) ::AGE::Log::CoreLogger()->info(__VA_ARGS__)
  #define AGE_CORE_WARN(...) ::AGE::Log::CoreLogger()->warn(__VA_ARGS__)
  #define AGE_CORE_ERROR(...) ::AGE::Log::CoreLogger()->error(__VA_ARGS__)
  #define AGE_CORE_CRITICAL(...) ::AGE::Log::CoreLogger()->critical(__VA_ARGS__)

  //Client log macros
  #define AGE_TRACE(...) ::AGE::Log::ClientLogger()->trace(__VA_ARGS__)
  #define AGE_INFO(...) ::AGE::Log::ClientLogger()->info(__VA_ARGS__)
  #define AGE_WARN(...) ::AGE::Log::CoreLogger()->warn(__VA_ARGS__)
  #define AGE_ERROR(...) ::AGE::Log::ClientLogger()->error(__VA_ARGS__)
  #define AGE_CRITICAL(...) ::AGE::Log::ClientLogger()->critical(__VA_ARGS__)
#else
  //Core logger macros
  #define AGE_CORE_TRACE(...)
  #define AGE_CORE_INFO(...)
  #define AGE_CORE_WARN(...)
  #define AGE_CORE_ERROR(...)
  #define AGE_CORE_CRITICAL(...)

  //Client logger macros
  #define AGE_TRACE(...)
  #define AGE_INFO(...)
  #define AGE_WARN(...)
  #define AGE_ERROR(...)
  #define AGE_CRITICAL(...)
#endif

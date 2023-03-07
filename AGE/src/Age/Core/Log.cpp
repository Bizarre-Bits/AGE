//
// Created by a on 24.08.22.
//

#include "agepch.h"

#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace AGE {
  std::shared_ptr<spdlog::logger> InitCoreLogger() {
    spdlog::set_pattern("%^[%T] %n |%l|: %v%$");

    auto coreLogger = spdlog::stdout_color_mt("Engine");
    coreLogger->set_level(spdlog::level::trace);
    return coreLogger;
  }

  std::shared_ptr<spdlog::logger> InitClientLogger() {
    spdlog::set_pattern("%^[%T] %n |%l|: %v%$");

    auto clientLogger = spdlog::stdout_color_mt("App");
    clientLogger->set_level(spdlog::level::trace);
    return clientLogger;
  }

  std::shared_ptr<spdlog::logger> Log::s_CoreLogger{InitCoreLogger()};
  std::shared_ptr<spdlog::logger> Log::s_ClientLogger{InitClientLogger()};

  void Log::Init() {
    AGE_PROFILE_FUNCTION();
  }

  std::shared_ptr<spdlog::logger>& Log::CoreLogger() {
    // TODO: That's not good. Should come up with some better solution
    // It may be a big performance problem in the future
    return s_CoreLogger;
  }

  std::shared_ptr<spdlog::logger>& Log::ClientLogger() {
    return s_ClientLogger;
  }
} // AGE
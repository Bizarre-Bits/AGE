//
// Created by a on 24.08.22.
//

#include "agepch.h"

#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace AGE {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
    AGE_PROFILE_FUNCTION();

		spdlog::set_pattern("%^[%T] %n |%l|: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("Engine");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
} // AGE
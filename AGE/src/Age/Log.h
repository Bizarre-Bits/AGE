//
// Created by a on 24.08.22.
//

#ifndef AGE_LOG_H
#define AGE_LOG_H

#include "Age/Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace AGE {

	class DLL_PUBLIC Log {
		public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }

		inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

		private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

} // AGE

#ifdef DEBUG
//Core log macros
	#define AGE_CORE_TRACE(...)       ::AGE::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define AGE_CORE_INFO(...)        ::AGE::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define AGE_CORE_ERROR(...)       ::AGE::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define AGE_CORE_CRITICAL(...)    ::AGE::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client log macros
	#define AGE_TRACE(...)            ::AGE::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define AGE_INFO(...)             ::AGE::Log::GetClientLogger()->info(__VA_ARGS__)
	#define AGE_ERROR(...)            ::AGE::Log::GetClientLogger()->error(__VA_ARGS__)
	#define AGE_CRITICAL(...)         ::AGE::Log::GetClientLogger()->critical(__VA_ARGS__)
#else
//Core logger macros
	#define AGE_CORE_TRACE(...)
	#define AGE_CORE_TRACE(...)
	#define AGE_CORE_INFO(...)
	#define AGE_CORE_ERROR(...)
	#define AGE_CORE_CRITICAL(...)

//Client logger macros
	#define AGE_TRACE(...)
	#define AGE_TRACE(...)
	#define AGE_INFO(...)
	#define AGE_ERROR(...)
	#define AGE_CRITICAL(...)
#endif

#endif //AGE_LOG_H

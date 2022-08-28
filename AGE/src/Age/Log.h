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
//Core logger macros
	#define CORE_LOG_TRACE(...)       ::AGE::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define CORE_LOG_DEBUG(...)       ::AGE::Log::GetCoreLogger()->debug(__VA_ARGS__)
	#define CORE_LOG_INFO(...)        ::AGE::Log::GetCoreLogger()->info(__VA_ARGS__)
	#define CORE_LOG_ERROR(...)       ::AGE::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define CORE_LOG_CRITICAL(...)    ::AGE::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client logger macros
	#define CLIENT_LOG_TRACE(...)     ::AGE::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define CLIENT_LOG_DEBUG(...)     ::AGE::Log::GetClientLogger()->debug(__VA_ARGS__)
	#define CLIENT_LOG_INFO(...)      ::AGE::Log::GetClientLogger()->info(__VA_ARGS__)
	#define CLIENT_LOG_ERROR(...)     ::AGE::Log::GetClientLogger()->error(__VA_ARGS__)
	#define CLIENT_LOG_CRITICAL(...)  ::AGE::Log::GetClientLogger()->critical(__VA_ARGS__)
#else
//Core logger macros
	#define CORE_LOG_TRACE(...)
	#define CORE_LOG_DEBUG(...)
	#define CORE_LOG_INFO(...)
	#define CORE_LOG_ERROR(...)
	#define CORE_LOG_CRITICAL(...)

//Client logger macros
	#define CLIENT_LOG_TRACE(...)
	#define CLIENT_LOG_DEBUG(...)
	#define CLIENT_LOG_INFO(...)
	#define CLIENT_LOG_ERROR(...)
	#define CLIENT_LOG_CRITICAL(...)
#endif

#endif //AGE_LOG_H

//
// Created by alex on 06.11.22.
//

#ifndef AGE_INSTRUMENTOR_H
#define AGE_INSTRUMENTOR_H

#include <chrono>
#include <thread>
#include <mutex>
#include <fstream>
#include <iomanip>

#include "Age/Core/Log.h"

namespace AGE {

  using DoubleMicrosecondsDuration = std::chrono::duration<double, std::micro>;

  struct ProfileResult {
    const char* Name;

    DoubleMicrosecondsDuration Start;
    std::chrono::microseconds  ElapsedTime;
    std::thread::id            ThreadID;
  };

  struct InstrumentationSession {
    const char* Name;
  };

  class Instrumentor {
  public:
    Instrumentor(const Instrumentor&) = delete;
    Instrumentor(Instrumentor&&) = delete;

    void BeginSession(const char* name, const char* filepath = "profile_results.json") {
      std::lock_guard lock(m_Mutex);
      if (m_CurrentSession) {
        if (Log::CoreLogger()) {
          AGE_CORE_ERROR("Instrumentor::BeginSession('{}') when session '{}' is already open.",
                         name, m_CurrentSession->Name);
        }
        InternalEndSession();
      }
      m_OutputStream.open(filepath);

      if (m_OutputStream.is_open()) {
        m_CurrentSession = new InstrumentationSession({name});
        WriteHeader();
      } else {
        if (Log::CoreLogger()) {
          AGE_CORE_ERROR("Instrumentor could not open results file '{}'", filepath);
        }
      }
    }

    void EndSession() {
      std::lock_guard lock(m_Mutex);
      InternalEndSession();
    }

    void WriteProfile(const ProfileResult& result) {
      std::stringstream json;
      json << std::setprecision(3) << std::fixed;
      json << ", {";
      json << R"("cat":"function",)";
      json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
      json << R"("name":")" << result.Name << "\",";
      json << R"("ph":"X",)";
      json << "\"pid\":0,";
      json << "\"tid\":" << result.ThreadID << ',';
      json << "\"ts\":" << result.Start.count();
      json << "}";

      std::lock_guard lock(m_Mutex);
      if (m_CurrentSession) {
        m_OutputStream << json.str();
        m_OutputStream.flush();
      }
    }

    static Instrumentor& Instance() {
      static Instrumentor instance;
      return instance;
    }

  private:
    Instrumentor() : m_CurrentSession(nullptr) {}

    void WriteHeader() {
      m_OutputStream << R"({"otherData": {},"traceEvents":[{})";
      m_OutputStream.flush();
    }

    void WriteFooter() {
      m_OutputStream << "]}";
      m_OutputStream.flush();
    }

    void InternalEndSession() {
      if (m_CurrentSession) {
        WriteFooter();
        m_OutputStream.close();
        delete m_CurrentSession;
        m_CurrentSession = nullptr;
      }
    }

  private:
    std::mutex m_Mutex;
    InstrumentationSession* m_CurrentSession;
    std::ofstream m_OutputStream;
  };

  class InstrumentorTimer {
  public:
    InstrumentorTimer(const char* name) : m_Name{name}, m_Stopped{false} {
      m_StartTimepoint = std::chrono::steady_clock::now();
    }

    ~InstrumentorTimer() {
      if (!m_Stopped) Stop();
    }

    void Stop() {
      auto endTimepoint = std::chrono::steady_clock::now();
      auto highResStart = DoubleMicrosecondsDuration(m_StartTimepoint.time_since_epoch());
      auto elapsedTime  = std::chrono::time_point_cast<std::chrono::microseconds>(
          endTimepoint
      ).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(
          m_StartTimepoint
      ).time_since_epoch();

      Instrumentor::Instance().WriteProfile(
          {m_Name, highResStart, elapsedTime, std::this_thread::get_id()}
      );

      m_Stopped = true;
    }

  private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
    bool                                               m_Stopped;
  };

  namespace InstrumentorUtils {

    template<size_t N>
    struct ChangeResult {
      char Data[N];
    };

    template<size_t N, size_t K>
    constexpr auto CleanupOutputString(const char(& expr)[N], const char(& remove)[K]) {
      ChangeResult<N> result = {};

      size_t srcIndex = 0;
      size_t dstIndex = 0;
      while (srcIndex < N) {
        size_t matchIndex = 0;
        while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1
               && expr[srcIndex + matchIndex] == remove[matchIndex])
          matchIndex++;
        if (matchIndex == K - 1)
          srcIndex += matchIndex;
        result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
        srcIndex++;
      }
      return result;
    }
  }

} // AGE

#if AGE_PROFILE
  #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
    #define AGE_FUNC_SIG __PRETTY_FUNCTION__
  #elif defined(__DMC__) && (__DMC__ >= 0x810)
    #define AGE_FUNC_SIG __PRETTY_FUNCTION__
  #elif (defined(__FUNCSIG__) || (_MSC_VER))
    #define AGE_FUNC_SIG __FUNCSIG__
  #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
    #define AGE_FUNC_SIG __FUNCTION__
  #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
    #define AGE_FUNC_SIG __FUNC__
  #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
    #define AGE_FUNC_SIG __func__
  #elif defined(__cplusplus) && (__cplusplus >= 201103)
    #define AGE_FUNC_SIG __func__
  #else
    #define AGE_FUNC_SIG "AGE_FUNC_SIG unknown!"
  #endif

  #define AGE_PROFILE_BEGIN_SESSION(name, filepath) ::AGE::Instrumentor::Instance().BeginSession(name, filepath)
  #define AGE_PROFILE_END_SESSION() ::AGE::Instrumentor::Instance().EndSession()
  #define AGE_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::AGE::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
											   ::AGE::InstrumentorTimer timer##line(fixedName##line.Data)
  #define AGE_PROFILE_SCOPE_LINE(name, line) AGE_PROFILE_SCOPE_LINE2(name, line)
  #define AGE_PROFILE_SCOPE(name) AGE_PROFILE_SCOPE_LINE(name, __LINE__)
  #define AGE_PROFILE_FUNCTION()
//  #define AGE_PROFILE_FUNCTION() AGE_PROFILE_SCOPE(AGE_FUNC_SIG)
#else
  #define AGE_PROFILE_BEGIN_SESSION(name, filepath)
	#define AGE_PROFILE_END_SESSION()
	#define AGE_PROFILE_SCOPE(name)
	#define AGE_PROFILE_FUNCTION()
#endif

#endif //AGE_INSTRUMENTOR_H

//
// Created by alex on 06.11.22.
//

#ifndef AGE_PROFILER_H
#define AGE_PROFILER_H

#include <concepts>

#include "Age/Core/Timer.h"

namespace AGE {
  template<typename F>
  concept Func = std::invocable<F&&, const char*, Timestep>;

  template<Func Fn>
  class Profiler {
  public:
    Profiler(const char* name, Fn&& func);
    ~Profiler();

    void Stop();

  private:
    const char* m_Name;
    Timer    m_Timer;
    Timestep m_Duration;
    bool     m_Stopped;
    Fn       m_Func;
  };

  struct ProfileResult {
    const char* Name;
    const float Duration;
  };

#ifdef DEBUG
  static std::vector<ProfileResult> s_ProfileResults;
  #define AGE_PROFILER_SCOPE(scopeName) ::AGE::Profiler profiler##__LINE__(scopeName, [&](const char* name, ::AGE::Timestep ts) {::AGE::s_ProfileResults.push_back({name, ts.Milliseconds()});})
  #define AGE_RESET_PROFILER() ::AGE::s_ProfileResults.clear();
#else
  #define AGE_PROFILER_SCOPE(scopeName)
  #define AGE_RESET_PROFILER()
#endif


} // AGE


// Implementation
namespace AGE {

  template<Func Fn>
  Profiler<Fn>::Profiler(const char* name, Fn&& func): m_Name{name}, m_Func{func},
                                                       m_Stopped{false}, m_Timer() {
    m_Timer.Start();
  }

  template<Func Fn>
  Profiler<Fn>::~Profiler() {
    if (!m_Stopped) {
      Stop();
      m_Func(m_Name, m_Duration);
    }
  }

  template<Func Fn>
  void Profiler<Fn>::Stop() {
    if (m_Stopped) return;
    m_Duration = m_Timer.DeltaTime();
    m_Stopped  = true;
  }
}

#endif //AGE_PROFILER_H

//
// Created by alex on 06.10.22.
//

#include "agepch.h"

#include "Timer.h"

namespace AGE {
  Timer::timepoint_t Timer::Start() {
    AGE_PROFILE_FUNCTION();

    m_Start    = clock_t::now();
    m_LastTime = m_Start;
    return m_Start;
  }

  Timestep Timer::DeltaTime() {
    AGE_PROFILE_FUNCTION();

    const timepoint_t now     = clock_t::now();
    duration_t        elapsed = now - m_LastTime;
    m_LastTime = now;

    return {elapsed.count()};
  }

  Timestep Timer::Uptime() const {
    AGE_PROFILE_FUNCTION();

    const timepoint_t now     = clock_t::now();
    duration_t        elapsed = now - m_Start;

    return {elapsed.count()};
  }
} // AGE
//
// Created by alex on 06.10.22.
//

#include "agepch.h"

#include "Timer.h"

namespace AGE {
  Timer::timepoint_t Timer::Start() {
    m_Start = clock_t::now();
    return m_Start;
  }

  Timestep Timer::DeltaTime() {
    const timepoint_t now     = clock_t::now();
    duration_t        elapsed = now - m_LastTime;
    m_LastTime = now;

    return {elapsed.count()};
  }

  Timestep Timer::Uptime() const {
    const timepoint_t now     = clock_t::now();
    duration_t        elapsed = now - m_Start;

    return {elapsed.count()};
  }
} // AGE
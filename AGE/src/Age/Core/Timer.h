//
// Created by alex on 06.10.22.
//

#pragma once

#include <chrono>

namespace AGE {

  class Timestep {
  public:
    Timestep(float time = 0.0f) : m_Time(time) {}

    inline float Seconds() const { return m_Time; }
    inline float Milliseconds() const { return m_Time * 1000; }

    operator float() const { return m_Time; }
  private:
    float m_Time;
  };

  class Timer {
  public:
    using clock_t = std::chrono::high_resolution_clock;
    using timepoint_t = clock_t::time_point;
    using duration_t = std::chrono::duration<float>;

    timepoint_t Start();
    Timestep DeltaTime();
    Timestep Uptime() const;

    static Timer GlobalClock() {
      static Timer GlobalClock;
      return GlobalClock;
    }

  private:
    timepoint_t m_Start;
    timepoint_t m_LastTime;
  };

} // AGE

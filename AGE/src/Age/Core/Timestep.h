//
// Created by alex on 06.10.22.
//

#ifndef AGE_TIMESTEP_H
#define AGE_TIMESTEP_H

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

} // AGE

#endif //AGE_TIMESTEP_H

//
// Created by alex on 1/25/23.
//

#ifndef AGE_PARTICLE_H
#define AGE_PARTICLE_H

#include <vector>

#include "glm/glm.hpp"

#include "Age/Age.h"

struct ParticleProps {
  glm::vec2 Position;
  glm::vec2 Velocity;
  float     VelocityVariations;
  float     RotationSpeedDeg;
  float     RotationSpeedVariationDeg;

  glm::vec4 ColorBegin, ColorEnd;
  float     ColorVariations;

  float SizeBegin, SizeEnd;
  float SizeVariations;

  float LifeTime{1.0f};
  float Rotation;
};

class ParticleSystem {
public:
  ParticleSystem(uint32_t poolSize) : m_Particles(poolSize), m_NextPoolInsert(m_Particles.begin()) {}

  void OnUpdate(AGE::Timestep ts);
  void Render();
  void Emit(const ParticleProps& props);

private:
  struct Particle {
    glm::vec2 Position;
    float     RotationDeg;
    float     RotationSpeedDeg;
    glm::vec2 Velocity;
    glm::vec4 ColorBegin, ColorEnd;
    float     SizeBegin, SizeEnd;

    float LifeTime{1.0f};
    float LifeRemaining{0.0f};

    bool Active{false};
  };

  std::vector<Particle>           m_Particles;
  std::vector<Particle>::iterator m_NextPoolInsert;
};


#endif //AGE_PARTICLE_H

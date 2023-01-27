//
// Created by alex on 1/25/23.
//

#include "Particle.h"

void ParticleSystem::OnUpdate(AGE::Timestep ts) {
  for (auto& particle: m_Particles) {
    if (!particle.Active)
      continue;

    if (particle.LifeRemaining <= 0.0f) {
      particle.Active = false;
      continue;
    }

    particle.LifeRemaining -= ts;
    particle.Position += particle.Velocity * ts.Seconds();
    particle.RotationDeg += particle.RotationSpeedDeg * ts.Seconds();
  }
}

void ParticleSystem::Render() {
  for (auto& particle: m_Particles) {
    if(!particle.Active)
      continue;

    float     life  = particle.LifeRemaining / particle.LifeTime;
    glm::vec4 color = particle.ColorBegin * (1.0f - life) + particle.ColorEnd * (life);
    float size = particle.SizeBegin * (1.0f + life) + particle.SizeEnd * (life);

    AGE::Renderer2D::DrawRotatedQuad(particle.Position, glm::vec2{size}, particle.RotationDeg, color);
  }
}

void ParticleSystem::Emit(const ParticleProps& props) {
  if (m_NextPoolInsert == m_Particles.end())
    m_NextPoolInsert = m_Particles.begin();

  glm::vec2 velocity  = {props.Velocity.x + props.VelocityVariations * AGE::Random::Float(-1.0f, 1.0f),
                         props.Velocity.y + props.VelocityVariations * AGE::Random::Float(-1.0f, 1.0f)};
  float rotationSpeed = props.RotationSpeedDeg + props.RotationSpeedVariationDeg * AGE::Random::Float(-1.0f, 1.0f);

  float     sizeBegin = props.SizeBegin + props.SizeVariations * AGE::Random::Float(-1.0f, 1.0f);
  float     sizeEnd   = props.SizeEnd + props.SizeVariations * AGE::Random::Float(-1.0f, 1.0f);

  glm::vec4 colorBegin = {props.ColorBegin.r + props.ColorVariations * AGE::Random::Float(-1.0f, 1.0f),
                          props.ColorBegin.g + props.ColorVariations * AGE::Random::Float(-1.0f, 1.0f),
                          props.ColorBegin.b + props.ColorVariations * AGE::Random::Float(-1.0f, 1.0f),
                          props.ColorBegin.a + props.ColorVariations * AGE::Random::Float(-1.0f, 1.0f)};
  glm::vec4 colorEnd = {props.ColorEnd.r + props.ColorVariations * AGE::Random::Float(-1.0f, 1.0f),
                          props.ColorEnd.g + props.ColorVariations * AGE::Random::Float(-1.0f, 1.0f),
                          props.ColorEnd.b + props.ColorVariations * AGE::Random::Float(-1.0f, 1.0f),
                          props.ColorEnd.a + props.ColorVariations * AGE::Random::Float(-1.0f, 1.0f)};

  m_NextPoolInsert->Active   = true;
  m_NextPoolInsert->LifeRemaining = m_NextPoolInsert->LifeTime = props.LifeTime;

  m_NextPoolInsert->ColorEnd   = colorBegin;
  m_NextPoolInsert->ColorBegin = colorEnd;

  m_NextPoolInsert->RotationSpeedDeg = rotationSpeed;
  m_NextPoolInsert->RotationDeg      = 0;
  m_NextPoolInsert->Position         = props.Position;
  m_NextPoolInsert->SizeBegin = sizeBegin;
  m_NextPoolInsert->SizeEnd   = sizeEnd;
  m_NextPoolInsert->Velocity  = velocity;

  m_NextPoolInsert++;
}

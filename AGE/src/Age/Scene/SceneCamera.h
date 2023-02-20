//
// Created by alex on 2/20/23.
//

#pragma once

#include "Age/Renderer/Camera.h"

namespace AGE {

  class SceneCamera : public Camera {
  public:
    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetOrthographic(float size, float nearClip, float farClip);
    void SetViewportSize(uint32_t width, uint32_t height);

    inline float GetOrthographicSize() const { return m_OrthographicSize; }
    inline void SetOrthographicSize(float size) { m_OrthographicSize = size; }

  private:
    void RecalculateProjection();

  private:
    float m_OrthographicSize{10.0f};
    float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
    float m_AspectRatio{0.0f};
  };

}// namespace AGE

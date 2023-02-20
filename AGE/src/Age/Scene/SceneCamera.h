//
// Created by alex on 2/20/23.
//

#pragma once

#include "Age/Renderer/Camera.h"

namespace AGE {

  class SceneCamera : public Camera {
  public:
    enum class ProjectionType {
      Perspective  = 0,
      Orthographic = 1
    };

  public:
    SceneCamera();
    virtual ~SceneCamera() = default;

    void SetViewportSize(uint32_t width, uint32_t height);

    // Orthographic Projection

    void SetOrthographic(float size, float nearClip, float farClip);

    [[nodiscard]] inline float GetOrthographicSize() const { return m_OrthographicSize; }
    inline void SetOrthographicSize(float size) {
      m_OrthographicSize = size;
      RecalculateProjection();
    }

    [[nodiscard]] inline float GetOrthographicNearClip() const { return m_OrthographicNear; }
    inline void SetOrthographicNearClip(float nearClip) {
      m_OrthographicNear = nearClip;
      RecalculateProjection();
    }

    [[nodiscard]] inline float GetOrthographicFarClip() const { return m_OrthographicFar; }
    inline void SetOrthographicFarClip(float farClip) {
      m_OrthographicFar = farClip;
      RecalculateProjection();
    }

    // Perspective Projection
    void SetPerspective(float fov, float nearClip, float farClip);

    inline float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
    inline void SetPerspectiveVerticalFOV(float size) {
      m_PerspectiveFOV = size;
      RecalculateProjection();
    }

    inline float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
    inline void SetPerspectiveNearClip(float nearClip) {
      m_PerspectiveNear = nearClip;
      RecalculateProjection();
    }

    inline float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
    inline void SetPerspectiveFarClip(float farClip) {
      m_PerspectiveFar = farClip;
      RecalculateProjection();
    }

    [[nodiscard]] inline ProjectionType GetProjectionType() const { return m_ProjectionType; }
    inline void SetProjectionType(ProjectionType projectionType) {
      m_ProjectionType = projectionType;
      RecalculateProjection();
    }

  private:
    void RecalculateProjection();

  private:
    float m_OrthographicSize{10.0f};
    float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;

    float m_PerspectiveFOV{glm::radians(45.0f)};
    float m_PerspectiveNear = 0.001f, m_PerspectiveFar = 1000.0f;

    float m_AspectRatio{0.0f};

    ProjectionType m_ProjectionType{ProjectionType::Orthographic};
  };

}// namespace AGE

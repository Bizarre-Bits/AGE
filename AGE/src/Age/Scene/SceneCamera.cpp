//
// Created by alex on 2/20/23.
//
#include "agepch.h"

#include "SceneCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace AGE {

  SceneCamera::SceneCamera() {
    RecalculateProjection();
  }

  void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
    m_OrthographicSize = size;
    m_OrthographicNear = nearClip;
    m_OrthographicFar  = farClip;

    RecalculateProjection();
  }

  void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
    m_AspectRatio = (float)width / (float)height;

    RecalculateProjection();
  }

  void SceneCamera::SetPerspective(float fov, float nearClip, float farClip) {
    m_PerspectiveFOV  = fov;
    m_PerspectiveNear = nearClip;
    m_PerspectiveFar  = farClip;

    RecalculateProjection();
  }

  void SceneCamera::RecalculateProjection() {
    if (m_ProjectionType == ProjectionType::Perspective) {
      m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
    } else {
      float orthoLeft   = -0.5f * m_AspectRatio * m_OrthographicSize;
      float orthoRight  = -orthoLeft;
      float orthoBottom = -0.5f * m_OrthographicSize;
      float orthoTop    = -orthoBottom;

      m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
    }
  }
}// namespace AGE
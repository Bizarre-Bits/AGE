//
// Created by alex on 18.10.22.
//

#include "agepch.h"

#include "OrthographicCameraController.h"
#include <glm/trigonometric.hpp>

#include "Age/Core/Core.h"
#include "Age/Core/Input.h"

namespace AGE {
  OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
      : m_AspectRatio(aspectRatio), m_ZoomLevel{1.0f},
        m_Bounds{-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel},
        m_Camera(
            -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel
        ),
        m_Rotation(rotation) {
  }

  void OrthographicCameraController::OnUpdate(Timestep ts) {
    AGE_PROFILE_FUNCTION();

    if (Input::IsKeyPressed(Key::A)) {
      m_CameraPosition.x -= cosf(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
      m_CameraPosition.y -= sinf(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    } else if (Input::IsKeyPressed(Key::D)) {
      m_CameraPosition.x += cosf(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
      m_CameraPosition.y += sinf(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }

    if (Input::IsKeyPressed(Key::W)) {
      m_CameraPosition.x += -sinf(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
      m_CameraPosition.y += cosf(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    } else if (Input::IsKeyPressed(Key::S)) {
      m_CameraPosition.x -= -sinf(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
      m_CameraPosition.y -= cosf(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
    }

    if (m_Rotation) {
      if (Input::IsKeyPressed(Key::Q))
        m_CameraRotation += m_CameraRotationSpeed * ts;
      if (Input::IsKeyPressed(Key::E))
        m_CameraRotation -= m_CameraRotationSpeed * ts;

      if (m_CameraRotation > 180.0f)
        m_CameraRotation -= 360.0f;
      else if (m_CameraRotation <= -180.0f)
        m_CameraRotation += 360.0f;

      m_Camera.SetRotation(m_CameraRotation);
    }

    m_Camera.SetPosition(m_CameraPosition);

    m_CameraTranslationSpeed = m_ZoomLevel;
  }

  void OrthographicCameraController::OnEvent(Event& e) {
    AGE_PROFILE_FUNCTION();

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(
        AGE_BIND_EVENT_FN(&OrthographicCameraController::OnMouseScrolled));
  }

  void OrthographicCameraController::OnResize(float width, float height) {
    AGE_PROFILE_FUNCTION();

    m_AspectRatio = width / height;
    CalculateView();
  }

  bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
    AGE_PROFILE_FUNCTION();

    m_ZoomLevel -= e.YOffset() * 0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
    CalculateView();
    return false;
  }

  bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
    AGE_PROFILE_FUNCTION();

    OnResize((float)e.Width(), (float)e.Height());
    return false;
  }

  void OrthographicCameraController::SetZoomLevel(float level) {
    m_ZoomLevel = level;
    CalculateView();
  }

  void OrthographicCameraController::CalculateView() {
    m_Bounds = {-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel};
    m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
  }
} // AGE

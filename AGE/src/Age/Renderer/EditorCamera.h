#pragma once

#include "Camera.h"
#include "Age/Core/Timer.h"
#include "Age/Events/Event.h"
#include "Age/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace AGE {

  class EditorCamera : public Camera {
  public:
    EditorCamera() = default;
    EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    inline float Distance() const { return m_Distance; }

    inline void SetDistance(float distance) { m_Distance = distance; }

    void SetViewportSize(float width, float height);

    inline const glm::mat4& ViewMatrix() const { return m_ViewMatrix; }

    inline glm::mat4 ViewProjection() const { return m_Projection * m_ViewMatrix; }

    glm::vec3 UpDirection() const;
    glm::vec3 RightDirection() const;
    glm::vec3 ForwardDirection() const;

    const glm::vec3& Position() const { return m_Position; }

    glm::quat Orientation() const;

    float Pitch() const { return m_Pitch; }

    float Yaw() const { return m_Yaw; }

  private:
    void UpdateProjection();
    void UpdateView();

    bool OnMouseScroll(MouseScrolledEvent& e);

    void MousePan(const glm::vec2& delta);
    void MouseRotate(const glm::vec2& delta);
    void MouseZoom(float delta);

    glm::vec3 CalculatePosition() const;

    std::pair<float, float> PanSpeed() const;
    float RotationSpeed() const;
    float ZoomSpeed() const;

  private:
    float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

    glm::mat4 m_ViewMatrix;
    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_FocalPoint = {0.0f, 0.0f, 0.0f};

    glm::vec2 m_InitialMousePosition = {0.0f, 0.0f};

    float m_Distance = 5.0f;
    float m_Pitch = 0.5f, m_Yaw = 0.0f;

    float m_ViewportWidth = 1280, m_ViewportHeight = 720;
  };

}

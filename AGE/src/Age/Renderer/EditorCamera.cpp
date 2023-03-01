#include "agepch.h"

#include "EditorCamera.h"
#include "Age/Core/Input.h"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

namespace AGE {

  EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
      : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip),
        Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)) {
    UpdateView();
  }

  void EditorCamera::UpdateProjection() {
    m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
    m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
  }

  void EditorCamera::UpdateView() {
    // m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
    m_Position = CalculatePosition();

    glm::quat orientation = Orientation();
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
    m_ViewMatrix = glm::inverse(m_ViewMatrix);
  }

  std::pair<float, float> EditorCamera::PanSpeed() const {
    float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

    float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return {xFactor, yFactor};
  }

  float EditorCamera::RotationSpeed() const {
    return 0.8f;
  }

  float EditorCamera::ZoomSpeed() const {
    float distance = m_Distance * 0.25f;
    distance = std::max(distance, 0.0f);
    float speed = distance * distance;
    speed = std::min(speed, 200.0f); // max speed = 100
    return speed;
  }

  void EditorCamera::OnUpdate(Timestep ts) {
    const glm::vec2& mouse{Input::MouseX(), Input::MouseY()};
    glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.006f;
    m_InitialMousePosition = mouse;

    if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle)) {
      if (Input::IsKeyPressed(Key::LeftAlt))
        MouseZoom(delta.y);
      else
        MousePan(delta);
    } else if (Input::IsMouseButtonPressed(Mouse::ButtonRight)) {
      ImGui::CaptureMouseFromApp(true);
      MouseRotate(delta);
    }


    UpdateView();
  }

  void EditorCamera::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(AGE_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
  }

  bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e) {
    float delta = e.YOffset() * 0.1f;
    MouseZoom(delta);
    UpdateView();
    return false;
  }

  void EditorCamera::MousePan(const glm::vec2& delta) {
    auto [xSpeed, ySpeed] = PanSpeed();
    m_FocalPoint += -RightDirection() * delta.x * xSpeed * m_Distance;
    m_FocalPoint += UpDirection() * delta.y * ySpeed * m_Distance;
  }

  void EditorCamera::MouseRotate(const glm::vec2& delta) {
    float yawSign = UpDirection().y < 0 ? -1.0f : 1.0f;
    m_Yaw += yawSign * delta.x * RotationSpeed();
    m_Pitch += delta.y * RotationSpeed();
  }

  void EditorCamera::MouseZoom(float delta) {
    m_Distance -= delta * ZoomSpeed();
    if (m_Distance < 1.0f) {
      m_FocalPoint += ForwardDirection();
      m_Distance = 1.0f;
    }
  }

  glm::vec3 EditorCamera::UpDirection() const {
    return glm::rotate(Orientation(), glm::vec3(0.0f, 1.0f, 0.0f));
  }

  glm::vec3 EditorCamera::RightDirection() const {
    return glm::rotate(Orientation(), glm::vec3(1.0f, 0.0f, 0.0f));
  }

  glm::vec3 EditorCamera::ForwardDirection() const {
    return glm::rotate(Orientation(), glm::vec3(0.0f, 0.0f, -1.0f));
  }

  glm::vec3 EditorCamera::CalculatePosition() const {
    return m_FocalPoint - ForwardDirection() * m_Distance;
  }

  glm::quat EditorCamera::Orientation() const {
    return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
  }

  void EditorCamera::SetViewportSize(float width, float height) {
    if (width <= 0 || height <= 0)
      return;
    m_ViewportWidth = width;
    m_ViewportHeight = height;
    UpdateProjection();
  }

}

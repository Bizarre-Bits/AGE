//
// Created by alex on 18.10.22.
//

#ifndef AGE_ORTHOGRAPHICCAMERACONTROLLER_H
#define AGE_ORTHOGRAPHICCAMERACONTROLLER_H

#include <glm/glm.hpp>

#include "Age/Core/Timer.h"
#include "Age/Events/Event.h"
#include "Age/Events/WindowEvent.h"
#include "Age/Events/MouseEvent.h"

#include "OrthographicCamera.h"

namespace AGE {

  class OrthographicCameraController {
  public:
    struct CameraBounds {
      float Left, Right;
      float Bottom, Top;

      [[nodiscard]] inline float Width() const { return Right - Left; }
      [[nodiscard]] inline float Height() const { return Top - Bottom; }
    };

  public:
    explicit OrthographicCameraController(float aspectRatio, bool rotation = false);

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    void OnResize(float width, float height);
    void CalculateView();

    OrthographicCamera& GetCamera() { return m_Camera; }
    const OrthographicCamera& GetCamera() const { return m_Camera; }

    float ZoomLevel() const { return m_ZoomLevel; }
    void SetZoomLevel(float level);
    [[nodiscard]] inline CameraBounds Bounds() const { return m_Bounds; }
    inline float AspectRatio() const { return m_AspectRatio; }

  private:
    bool OnMouseScrolled(MouseScrolledEvent& e);
    bool OnWindowResized(WindowResizeEvent& e);

  private:
    float              m_AspectRatio;
    float              m_ZoomLevel = 1.0f;
    OrthographicCamera m_Camera;
    CameraBounds       m_Bounds;

    bool m_Rotation;

    glm::vec3 m_CameraPosition         = {0.0f, 0.0f, 0.0f};
    float     m_CameraRotation         = 0.0f; //In degrees, in the anti-clockwise direction
    float     m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
  };
} // AGE

#endif //AGE_ORTHOGRAPHICCAMERACONTROLLER_H

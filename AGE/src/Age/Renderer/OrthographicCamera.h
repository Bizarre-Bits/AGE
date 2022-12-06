//
// Created by alex on 05.10.22.
//

#ifndef AGE_ORTHOGRAPHICCAMERA_H
#define AGE_ORTHOGRAPHICCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AGE {

  class OrthographicCamera {
  public:
    OrthographicCamera(float left, float right, float bottom, float top);

    inline void SetPosition(const glm::vec3& position) {
      m_Position = position;
      RecalculateViewMatrix();
    }

    inline void SetRotation(const float rotation) {
      m_Rotation = rotation;
      RecalculateViewMatrix();
    }

    inline void SetProjection(float left, float right, float bottom, float top) {
      m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    }

    inline glm::vec3 Position() const { return m_Position; }
    inline float Rotation() const { return m_Rotation; }

    inline const glm::mat4& ProjectionMatrix() const { return m_ProjectionMatrix; }
    inline const glm::mat4& ViewMatrix() const { return m_ViewMatrix; }
    inline const glm::mat4& ViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

  private:
    void RecalculateViewMatrix();

  public:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position;
    float     m_Rotation;
  };

} // AGE

#endif //AGE_ORTHOGRAPHICCAMERA_H

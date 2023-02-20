//
// Created by alex on 2/15/23.
//

#pragma once
#include "agepch.h"

#include <Age/Renderer/Camera.h>
#include <glm/glm.hpp>

#include "Age/Renderer/OrthographicCamera.h"

namespace AGE {
  struct TagComponent {
    age_string_t Tag;
    TagComponent()                    = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const age_string_t& tag) : Tag{tag} {}
  };

  struct TransformComponent {
    glm::mat4 Transform{1.0f};

    TransformComponent()                          = default;
    TransformComponent(const TransformComponent&) = default;
    explicit TransformComponent(const glm::mat4& transform) : Transform{transform} {}

    operator const glm::mat4&() { return Transform; }
  };

  struct SpriteComponent {
    glm::vec4 Tint{1.0f};
  };

  struct CameraComponent {
    class Camera Camera;
    bool Primary = false; // TODO: Probably move this to scene

    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
    CameraComponent(const glm::mat4 projection) : Camera(projection) {}
  };
}// namespace AGE

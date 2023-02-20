//
// Created by alex on 2/15/23.
//

#pragma once
#include "agepch.h"

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace AGE {
  struct TagComponent {
    age_string_t Tag;
    TagComponent()                    = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const age_string_t& tag) : Tag{tag} {}
  };

  struct TransformComponent {
    glm::vec3 Translation{0.0f};
    glm::vec3 Rotation{0.0f};
    glm::vec3 Scale{1.0f};

    TransformComponent()                          = default;
    TransformComponent(const TransformComponent&) = default;
    explicit TransformComponent(const glm::vec3 translation) : Translation{translation} {}

    glm::mat4 Transform() const {
      glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, {1, 0, 0})
                           * glm::rotate(glm::mat4(1.0f), Rotation.y, {0, 1, 0})
                           * glm::rotate(glm::mat4(1.0f), Rotation.z, {0, 0, 1});

      return glm::translate(glm::mat4{1.0f}, Translation) * rotation * glm::scale(glm::mat4{1.0f}, Scale);
    }
  };

  struct SpriteComponent {
    glm::vec4 Tint{1.0f};
  };

  struct CameraComponent {
    class SceneCamera Camera;
    bool Primary{false};// TODO: Probably move this to scene
    bool FixedAspectRatio{false};

    CameraComponent() = default;

    CameraComponent(const CameraComponent&) = default;
  };

  struct NativeScriptComponent {
    ScriptableEntity* Instance{nullptr};

    ScriptableEntity* (*InstantiateScript)();
    void (*DestroyScript)(NativeScriptComponent*);

    template<typename T>
      requires std::derived_from<T, ScriptableEntity>
    void Bind() {
      InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
      DestroyScript     = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
    }
  };
}// namespace AGE

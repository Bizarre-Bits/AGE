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
    class SceneCamera Camera;
    bool Primary{false};// TODO: Probably move this to scene
    bool FixedAspectRatio{false};

    CameraComponent() = default;

    CameraComponent(const CameraComponent&) = default;
  };

  struct NativeScriptComponent {
    ScriptableEntity* Instance{nullptr};

    std::function<void()> InstantiateFunction;
    std::function<void()> DestroyInstanceFunction;

    std::function<void(ScriptableEntity* Instance)> OnCreateFunction;
    std::function<void(ScriptableEntity* Instance, Timestep ts)> OnUpdateFunction;
    std::function<void(ScriptableEntity* Instance)> OnDestroyFunction;

    template<typename T>
      requires std::derived_from<T, ScriptableEntity>
    void Bind() {
      InstantiateFunction     = [&]() { Instance = new T(); };
      DestroyInstanceFunction = [&]() { delete (T*)Instance; Instance = nullptr; };

      OnCreateFunction  = [](ScriptableEntity* instance) { instance->OnCreate(); };
      OnUpdateFunction  = [](ScriptableEntity* instance, Timestep ts) { instance->OnUpdate(ts); };
      OnDestroyFunction = [](ScriptableEntity* instance) { instance->OnDestroy(); };
    }
  };
}// namespace AGE

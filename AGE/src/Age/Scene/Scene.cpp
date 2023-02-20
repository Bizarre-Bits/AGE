//
// Created by alex on 2/15/23.
//

#include "Scene.h"
#include "Age/Renderer/Renderer2D.h"
#include "Components.h"
#include "Entity.h"

namespace AGE {
  Scene::Scene() {
  }

  Entity Scene::CreateEntity(const age_string_t& name) {
    Entity entity{m_Registry.create(), this};
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag   = name.empty() ? "Entity" : name;

    return entity;
  }

  void Scene::OnUpdate(Timestep ts) {
    // Update scripts
    {
      m_Registry.view<NativeScriptComponent>().each([this, ts](auto entity, NativeScriptComponent& nsc) {
        if (!nsc.Instance) {
          nsc.Instance           = nsc.InstantiateScript();
          nsc.Instance->m_Entity = Entity{entity, this};
          nsc.Instance->OnCreate();
        }

        nsc.Instance->OnUpdate(ts);
      });
    }

    // Render sprites
    Camera* mainCamera = nullptr;
    glm::mat4 mainCameraTransform{};

    auto cameras = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
    for (auto entity: cameras) {
      auto [camera, transform] = cameras.get<CameraComponent, TransformComponent>(entity);
      if (camera.Primary) {
        mainCamera          = &camera.Camera;
        mainCameraTransform = transform.Transform();
      }
    }


    if (!mainCamera)
      return;

    Renderer2D::BeginScene(*mainCamera, mainCameraTransform);

    auto sprites = m_Registry.group<SpriteComponent>(entt::get<TransformComponent>);
    for (auto e: sprites) {
      auto [sprite, transform] = sprites.get<SpriteComponent, TransformComponent>(e);
      Renderer2D::DrawQuad(transform.Transform(), sprite.Tint);
    }

    Renderer2D::EndScene();
  }

  void Scene::OnViewportResize(uint32_t width, uint32_t height) {
    auto view = m_Registry.view<CameraComponent>();
    for (auto& cameraEntity: view) {
      auto& cameraComponent = view.get<CameraComponent>(cameraEntity);
      if (cameraComponent.FixedAspectRatio)
        continue;

      cameraComponent.Camera.SetViewportSize(width, height);
    }
  }
}// namespace AGE
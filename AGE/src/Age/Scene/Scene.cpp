//
// Created by alex on 2/15/23.
//

#include "Scene.h"
#include "Age/Renderer/Renderer2D.h"
#include "Entity.h"
#include "Components.h"

namespace AGE {
  Scene::Scene() {
  }

  Entity Scene::CreateEntity(const age_string_t& name) {
    Entity entity{m_Registry.create(), this};
    entity.AddComponent<TransformComponent>();
    auto& tag = entity.AddComponent<TagComponent>();
    tag.Tag = name.empty() ? "Entity" : name;

    return entity;
  }

  void Scene::OnUpdate(Timestep ts) {
    // Render sprites
    Camera* mainCamera = nullptr;
    glm::mat4* mainCameraTransform = nullptr;
    {
      auto group = m_Registry.group<CameraComponent, TransformComponent>();
      for(auto entity : group) {
        auto [camera, transform] = group.get<CameraComponent, TransformComponent>(entity);
        if(camera.Primary) {
          mainCamera = &camera.Camera;
          mainCameraTransform = &transform.Transform;
        }
      }
    }

    if(!mainCamera)
      return;

    Renderer2D::BeginScene(*mainCamera, *mainCameraTransform);

    auto sprites = m_Registry.group<SpriteComponent>(entt::get<TransformComponent>);
    for (auto e: sprites) {
      auto [transform, sprite] = sprites.get<TransformComponent, SpriteComponent>(e);
      Renderer2D::DrawQuad(transform,sprite.Tint);
    }

    Renderer2D::EndScene();
  }
}// namespace AGE
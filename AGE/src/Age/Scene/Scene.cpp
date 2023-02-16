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
    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
    for (auto e: group) {
      auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(e);
      Renderer2D::DrawQuad(transform,sprite.Tint);
    }
  }
}// namespace AGE
//
// Created by alex on 2/15/23.
//

#include "Scene.h"
#include "Age/Renderer/Renderer2D.h"

namespace AGE {
  Scene::Scene() {
  }

  entt::entity Scene::CreateEntity() {
    return m_Registry.create();
  }

  void Scene::OnUpdate(Timestep ts) {
    auto group = m_Registry.group<TransformComponent>(entt::get<SpriteComponent>);
    for (auto e: group) {
      auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(e);
      Renderer2D::DrawQuad(transform,sprite.Tint);
    }
  }
}// namespace AGE
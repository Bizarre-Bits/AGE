//
// Created by alex on 2/20/23.
//

#include "SceneOutlinePanel.h"

#include <glm/gtc/type_ptr.hpp>

namespace AGE {
  SceneOutlinePanel::SceneOutlinePanel(const Ref<Scene>& scene) {
    SetContext(scene);
  }

  void SceneOutlinePanel::OnUiRender() {
    AGE_ASSERT(m_Context, "There is no scene context provided");

    ImGui::Begin("Scene Outline");
    m_Context->m_Registry.each([&](auto e) {
      Entity entity{e, m_Context.get()};
      DrawEntityNode(entity);
    });

    if(ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
      m_SelectedEntity = Entity::Null;

    ImGui::End();

    ImGui::Begin("Inspector");
    if (m_SelectedEntity) {
      DrawComponents(m_SelectedEntity);
    }
    ImGui::End();
  }

  void SceneOutlinePanel::DrawEntityNode(Entity& entity) {
    auto& tag                      = entity.GetComponent<TagComponent>().Tag;
    ImGuiTreeNodeFlags const flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool const opened              = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());

    if (ImGui::IsItemClicked()) {
      m_SelectedEntity = entity;
    }

    if (opened) {
      ImGui::TreePop();
    }
  }

  void SceneOutlinePanel::DrawComponents(Entity& entity) {
    if (entity.HasComponent<TagComponent>()) {
      auto& tag = entity.GetComponent<TagComponent>().Tag;
      char buffer[256];
      memset(buffer, 0, sizeof(buffer));
      memcpy(buffer, tag.c_str(), tag.size());
      if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
        tag = buffer;
      }
    }

    if (entity.HasComponent<TransformComponent>()) {
      if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform Component")) {
        auto& transform = entity.GetComponent<TransformComponent>().Transform;
        ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
      }
    }
  }
}// namespace AGE
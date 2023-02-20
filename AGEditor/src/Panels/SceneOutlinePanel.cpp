//
// Created by alex on 2/20/23.
//

#include "SceneOutlinePanel.h"

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

    ImGui::End();
  }

  void SceneOutlinePanel::DrawEntityNode(Entity& entity) {
    auto& tag                = entity.GetComponent<TagComponent>().Tag;
    ImGuiTreeNodeFlags const flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool const opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());

    if (ImGui::IsItemClicked()) {
      m_SelectedEntity = entity;
    }

    if(opened) {
      ImGui::TreePop();
    }
  }
}// namespace AGE
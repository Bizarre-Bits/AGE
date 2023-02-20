//
// Created by alex on 2/20/23.
//

#pragma once

#include <Age/Age.h>

namespace AGE {

  class SceneOutlinePanel {
  public:
    SceneOutlinePanel() = default;
    SceneOutlinePanel(const Ref<Scene>& scene);
    ~SceneOutlinePanel() = default;

    inline void SetContext(const Ref<Scene>& scene) { m_Context = scene; }

    void OnUiRender();

  private:
    void EntityNode(Entity& entity);

    template<typename T>
    void DrawComponentInspector(const age_string_t& inspectorName, std::function<void(T& component)> callback);

    void Inspector();
    void CameraComponentInspector(CameraComponent& component) const;
    void TransformComponentInspector(TransformComponent& component);
    void TagComponentInspector(TagComponent& component);

  private:
    Ref<Scene> m_Context;
    Entity m_SelectedEntity;

    friend class Scene;
  };

  template<typename T>
  void SceneOutlinePanel::DrawComponentInspector(const age_string_t& inspectorName, std::function<void(T& component)> callback) {
    if (m_SelectedEntity.HasComponent<T>()) {
      if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "%s", inspectorName.c_str())) {
        T& component = m_SelectedEntity.GetComponent<T>();
        callback(component);

        ImGui::TreePop();
      }
    }
  }

}// namespace AGE

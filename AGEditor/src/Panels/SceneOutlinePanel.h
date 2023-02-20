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

    bool Vec3Control(const age_string_t& label, glm::vec3& values, float resetValue = 0.0f, float speed = 1.0f, float columnWidth = 100.0f);

  private:
    Ref<Scene> m_Context;
    Entity m_SelectedEntity;

    friend class Scene;
  };

  template<typename T>
  void SceneOutlinePanel::DrawComponentInspector(const age_string_t& inspectorName, std::function<void(T& component)> callback) {
    if (m_SelectedEntity.HasComponent<T>()) {
      const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
      bool open                      = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, "%s", inspectorName.c_str());

      ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);

      if (ImGui::Button("+", ImVec2{20, 20}))
        ImGui::OpenPopup("ComponentSettings");

      bool deleteComponent{false};
      if (ImGui::BeginPopup("ComponentSettings")) {
        if(ImGui::MenuItem("Remove Component"))
          deleteComponent = true;

        ImGui::EndPopup();
      }

      if (open) {
        T& component = m_SelectedEntity.GetComponent<T>();
        callback(component);

        ImGui::TreePop();
      }
      if(deleteComponent)
        m_SelectedEntity.RemoveComponent<T>();
    }
  }

}// namespace AGE

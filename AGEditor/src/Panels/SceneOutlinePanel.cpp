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
      EntityNode(entity);
    });

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
      m_SelectedEntity = Entity::Null;

    ImGui::End();

    ImGui::Begin("Inspector");
    if (m_SelectedEntity) {
      Inspector();
    }
    ImGui::End();
  }

  void SceneOutlinePanel::EntityNode(Entity& entity) {
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

  void SceneOutlinePanel::Inspector() {
    DrawComponentInspector<TagComponent>("Tag", [this](TagComponent& component) {
      TagComponentInspector(component);
    });

    DrawComponentInspector<TransformComponent>("Transform", [this](TransformComponent& component) {
      TransformComponentInspector(component);
    });

    DrawComponentInspector<SpriteComponent>("Sprite", [](SpriteComponent& component) {
      glm::vec4& tint = component.Tint;
      ImGui::ColorEdit4("Tint", glm::value_ptr(tint));
    });


    DrawComponentInspector<CameraComponent>("Camera", [this](CameraComponent& component) {
      CameraComponentInspector(component);
    });
  }

  void SceneOutlinePanel::TransformComponentInspector(TransformComponent& component) {
    auto& transform = component.Transform;
    ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
  }

  void SceneOutlinePanel::TagComponentInspector(TagComponent& component) {
    auto& tag = component.Tag;
    char buffer[256];
    memset(&buffer[0], 0, sizeof(buffer));
    memcpy(&buffer[0], tag.c_str(), sizeof(buffer));
    if (ImGui::InputText("Tag", &buffer[0], sizeof(buffer))) {
      tag = age_string_t(buffer);
    }
  }

  void SceneOutlinePanel::CameraComponentInspector(CameraComponent& component) const {
    auto& camera = component.Camera;

    const char* projectionTypes[] = {"Perspective", "Orthographic"};
    if (ImGui::BeginCombo("Projection", projectionTypes[(int)camera.GetProjectionType()])) {
      for (int i = 0; i < 2; i++) {
        bool isSelected = i == (int)camera.GetProjectionType();

        if (ImGui::Selectable(projectionTypes[i], isSelected)) {
          camera.SetProjectionType((SceneCamera::ProjectionType)i);

          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
      }

      ImGui::EndCombo();
    }

    if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {

      float size{camera.GetOrthographicSize()}, near{camera.GetOrthographicNearClip()}, far{camera.GetOrthographicFarClip()};
      if (ImGui::DragFloat("Size", &size, 1.0f, 0.0f))
        camera.SetOrthographicSize(size);

      if (ImGui::DragFloat("Near Clip", &near, 1.0f, 0.0f, far))
        camera.SetOrthographicNearClip(near);

      if (ImGui::DragFloat("Far Clip", &far, 1.0f, near))
        camera.SetOrthographicFarClip(far);
    }

    if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
      float fov{glm::degrees(camera.GetPerspectiveVerticalFOV())}, near{camera.GetPerspectiveNearClip()}, far{camera.GetPerspectiveFarClip()};
      if (ImGui::DragFloat("FOV", &fov, 1.0f, 0.0f))
        camera.SetPerspectiveVerticalFOV(glm::radians(fov));

      if (ImGui::DragFloat("Near Clip", &near, 1.0f))
        camera.SetPerspectiveNearClip(near);

      if (ImGui::DragFloat("Far Clip", &far, 1.0f))
        camera.SetPerspectiveFarClip(far);
    }
  }
}// namespace AGE
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

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
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
      memset(&buffer[0], 0, sizeof(buffer));
      memcpy(&buffer[0], tag.c_str(), sizeof(buffer));
      if (ImGui::InputText("Tag", &buffer[0], sizeof(buffer))) {
        tag = age_string_t(buffer);
      }
    }

    if (entity.HasComponent<TransformComponent>()) {
      if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform")) {
        auto& transform = entity.GetComponent<TransformComponent>().Transform;
        ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

        ImGui::TreePop();
      }
    }

    if (entity.HasComponent<CameraComponent>()) {
      if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
        auto& cameraComponent = entity.GetComponent<CameraComponent>();

        const char* projectionTypes[] = {"Perspective", "Orthographic"};
        if (ImGui::BeginCombo("Projection", projectionTypes[(int)cameraComponent.Camera.GetProjectionType()])) {
          for (int i = 0; i < 2; i++) {
            bool isSelected = i == (int)cameraComponent.Camera.GetProjectionType();

            if (ImGui::Selectable(projectionTypes[i], isSelected)) {
              cameraComponent.Camera.SetProjectionType((SceneCamera::ProjectionType)i);

              if (isSelected)
                ImGui::SetItemDefaultFocus();
            }
          }

          ImGui::EndCombo();
        }

        if (cameraComponent.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {
          auto& camera = cameraComponent.Camera;

          float size{camera.GetOrthographicSize()}, near{camera.GetOrthographicNearClip()}, far{camera.GetOrthographicFarClip()};
          if (ImGui::DragFloat("Size", &size, 1.0f, 0.0f))
            camera.SetOrthographicSize(size);

          if (ImGui::DragFloat("Near Clip", &near, 1.0f, 0.0f, far))
            camera.SetOrthographicNearClip(near);

          if (ImGui::DragFloat("Far Clip", &far, 1.0f, near))
            camera.SetOrthographicFarClip(far);
        }

        if (cameraComponent.Camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {
          auto& camera = cameraComponent.Camera;

          float fov{glm::degrees(camera.GetPerspectiveVerticalFOV())}, near{camera.GetPerspectiveNearClip()}, far{camera.GetPerspectiveFarClip()};
          if (ImGui::DragFloat("FOV", &fov, 1.0f, 0.0f))
            camera.SetPerspectiveVerticalFOV(glm::radians(fov));

          if (ImGui::DragFloat("Near Clip", &near, 1.0f))
            camera.SetPerspectiveNearClip(near);

          if (ImGui::DragFloat("Far Clip", &far, 1.0f))
            camera.SetPerspectiveFarClip(far);
        }

        ImGui::TreePop();
      }
    }
  }
}// namespace AGE
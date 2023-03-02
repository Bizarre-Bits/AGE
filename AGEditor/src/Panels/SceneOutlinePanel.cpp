//
// Created by alex on 2/20/23.
//

#include "SceneOutlinePanel.h"

#include "imgui_internal.h"
#include <glm/gtc/type_ptr.hpp>

namespace AGE {
  SceneOutlinePanel::SceneOutlinePanel(const Ref<Scene>& scene) {
    SetContext(scene);
  }

  void SceneOutlinePanel::OnUiRender() {
    AGE_ASSERT(m_Context, "There is no scene context provided");

    ImGui::Begin("Scene Outline");

    // Right click on blank space
    if (ImGui::BeginPopupContextWindow()) {
      if (ImGui::MenuItem("Create Empty Entity"))
        m_Context->CreateEntity("Entity");

      ImGui::EndPopup();
    }

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
    auto& tag                = entity.GetComponent<TagComponent>().Tag;
    ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

    bool const opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());

    if (ImGui::IsItemClicked()) {
      m_SelectedEntity = entity;
    }

    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem()) {
      if (ImGui::MenuItem("Delete Entity"))
        entityDeleted = true;

      ImGui::EndPopup();
    }

    if (opened) {
      ImGui::TreePop();
    }

    if (entityDeleted) {
      m_Context->DestroyEntity(entity);
      m_SelectedEntity = Entity::Null;
    }
  }

  void SceneOutlinePanel::Inspector() {
    if (m_SelectedEntity.HasComponent<TagComponent>()) {
      auto& tag = m_SelectedEntity.GetComponent<TagComponent>().Tag;
      char buffer[256];
      memset(&buffer[0], 0, sizeof(buffer));
      memcpy(&buffer[0], tag.c_str(), sizeof(buffer));
      if (ImGui::InputText("##Tag", &buffer[0], sizeof(buffer))) {
        tag = age_string_t(buffer);
      }
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component"))
      ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent")) {
      if (ImGui::MenuItem("Camera"))
        m_SelectedEntity.AddComponent<CameraComponent>();

      if (ImGui::MenuItem("Sprite"))
        m_SelectedEntity.AddComponent<SpriteComponent>();

      ImGui::EndPopup();
    }

    ImGui::PopItemWidth();

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
    Vec3Control("Position", component.Translation, 0, 0.1f);

    glm::vec3 rotationDeg{glm::degrees(component.Rotation.x), glm::degrees(component.Rotation.y),
                          glm::degrees(component.Rotation.z)};
    if (Vec3Control("Rotation", rotationDeg))
      component.Rotation = {glm::radians(rotationDeg.x), glm::radians(rotationDeg.y),
                            glm::radians(rotationDeg.z)};

    Vec3Control("Scale", component.Scale, 1.0f, 0.1f);
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

    ImGui::Checkbox("Primary", &component.Primary);

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

  bool SceneOutlinePanel::Vec3Control(const age_string_t& label, glm::vec3& values, float resetValue, float speed,
                                      float columnWidth) {
    ImGuiIO& io      = ImGui::GetIO();
    ImFont* boldFont = io.Fonts->Fonts[0];
    bool isChanged{false};

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
    const float lineHeight = GImGui->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    const ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.2f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.3f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.2f, 0.2f, 1.0f));
    ImGui::PushFont(boldFont);

    if (ImGui::Button("X", buttonSize)) {
      values.x  = resetValue;
      isChanged = true;
    }

    ImGui::PopStyleColor(3);
    ImGui::PopFont();

    ImGui::SameLine();
    if (ImGui::DragFloat("##X", &values.x, speed))
      isChanged = true;

    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.6f, 0.2f, 1.0f));
    ImGui::PushFont(boldFont);

    if (ImGui::Button("Y", buttonSize)) {
      values.y  = resetValue;
      isChanged = true;
    }
    ImGui::PopStyleColor(3);
    ImGui::PopFont();

    ImGui::SameLine();
    if (ImGui::DragFloat("##Y", &values.y, speed))
      isChanged = true;

    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.6f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.8f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.6f, 1.0f));
    ImGui::PushFont(boldFont);

    if (ImGui::Button("Z", buttonSize)) {
      values.z  = resetValue;
      isChanged = true;
    }
    ImGui::PopStyleColor(3);
    ImGui::PopFont();

    ImGui::SameLine();
    if (ImGui::DragFloat("##Z", &values.z, speed))
      isChanged = true;

    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::PopID();

    ImGui::Columns(1);

    return isChanged;
  }

  void SceneOutlinePanel::SetContext(const Ref<Scene>& scene) {
    m_Context        = scene;
    m_SelectedEntity = Entity::Null;
  }

  void SceneOutlinePanel::SelectEntity(const Entity& entity) {
    m_SelectedEntity = entity;
  }

}// namespace AGE
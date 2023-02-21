//
// Created by alex on 2/21/23.
//

#include "agepch.h"

#include <yaml-cpp/yaml.h>

#include "Age/Scene/SerializationExpansions.h"
#include "Components.h"
#include "SceneSerializer.h"

namespace AGE {

  SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene{scene} {
  }

  void SceneSerializer::Serialize(const age_string_t& filepath) {
    AGE_CORE_TRACE("Serializing scene to \"{}\"", filepath);

    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "Scene" << YAML::Value << "Untitled";
    out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
    m_Scene->m_Registry.each([&](auto entityID) {
      AGE_CORE_TRACE("Serializing entity {} to {}", (uint32_t)entityID, filepath);
      Entity entity{entityID, m_Scene.get()};
      SerializeEntity(out, entity);
    });
    out << YAML::EndMap;

    std::ofstream fout{filepath};
    fout << out.c_str();
  }

  void SceneSerializer::SerializeRuntime(const age_string_t& filepath) {
    AGE_CORE_ASSERT(false, "Runtime serialization is not implemented yet");
  }

  bool SceneSerializer::Deserialize(const age_string_t& filepath) {
    std::ifstream ifstream(filepath);
    std::stringstream strStream;
    strStream << ifstream.rdbuf();

    YAML::Node data = YAML::Load(strStream.str());
    if (!data["Scene"])
      return false;

    age_string_t sceneName = data["Scene"].as<age_string_t>();
    AGE_CORE_TRACE("Deserializing scene '{0}'", sceneName);

    auto entities = data["Entities"];

    if (entities) {
      for (auto entity: entities) {
        uint64_t uuid = entity["EntityID"].as<uint64_t>();// TODO: Add uuid to entities
        age_string_t name;
        auto tagComponent = entity["TagComponent"];
        if (tagComponent)
          name = tagComponent["Tag"].as<age_string_t>();

        AGE_CORE_TRACE("Deserializing entity \"{0}\"({1})", name, uuid);

        Entity deserializedEntity = m_Scene->CreateEntity(name);

#pragma region TransformComponent

        auto transformComponent = entity["TransformComponent"];
        if (transformComponent) {
          auto& tc       = deserializedEntity.GetComponent<TransformComponent>();
          tc.Translation = transformComponent["Translation"].as<glm::vec3>();
          tc.Rotation    = transformComponent["Rotation"].as<glm::vec3>();
          tc.Scale       = transformComponent["Scale"].as<glm::vec3>();
        }// TransformComponent

#pragma endregion

#pragma region CameraComponent

        auto cameraComponent = entity["CameraComponent"];
        if (cameraComponent) {
          auto& cc = deserializedEntity.AddComponent<CameraComponent>();

          auto cameraProps = cameraComponent["Camera"];

          cc.Primary          = cameraComponent["Primary"].as<bool>();
          cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();

          cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
          {
            auto fov      = cameraProps["FOV"].as<float>();
            auto nearClip = cameraProps["PerspectiveNearClip"].as<float>();
            auto farClip  = cameraProps["PerspectiveFarClip"].as<float>();
            cc.Camera.SetPerspective(fov, nearClip, farClip);
          }
          {
            auto size     = cameraProps["OrthographicSize"].as<float>();
            auto nearClip = cameraProps["OrthographicNearClip"].as<float>();
            auto farClip  = cameraProps["OrthographicFarClip"].as<float>();
            cc.Camera.SetOrthographic(size, nearClip, farClip);
          }
        }// CameraComponent

#pragma endregion

#pragma region Sprite Component

        auto spriteComponent = entity["SpriteComponent"];
        if (spriteComponent) {
          auto& sc = deserializedEntity.AddComponent<SpriteComponent>();
          sc.Tint  = spriteComponent["Tint"].as<glm::vec4>();
        }

#pragma endregion
      }
    }

    return true;
  }

  bool SceneSerializer::DeserializeRuntime(const age_string_t& filepath) {
    AGE_CORE_ASSERT(false, "Runtime deserialization is not implemented yet");
    return false;
  }

  void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity& entity) {
    out << YAML::BeginMap;
    out << YAML::Key << "EntityID" << YAML::Value << "123148128998124124";

    if (entity.HasComponent<TagComponent>()) {
      out << YAML::Key << "TagComponent";
      out << YAML::BeginMap;

      auto& tag = entity.GetComponent<TagComponent>().Tag;
      out << YAML::Key << "Tag" << YAML::Value << tag;
      out << YAML::EndMap;
    }
    if (entity.HasComponent<TransformComponent>()) {
      out << YAML::Key << "TransformComponent";
      out << YAML::BeginMap;
      auto& transform = entity.GetComponent<TransformComponent>();
      out << YAML::Key << "Translation" << YAML::Value << transform.Translation;
      out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
      out << YAML::Key << "Scale" << YAML::Value << transform.Scale;
      out << YAML::EndMap;
    }
    if (entity.HasComponent<SpriteComponent>()) {
      out << YAML::Key << "SpriteComponent";
      out << YAML::BeginMap;
      auto& sprite = entity.GetComponent<SpriteComponent>();
      out << YAML::Key << "Tint" << YAML::Value << sprite.Tint;
      out << YAML::EndMap;
    }
    if (entity.HasComponent<CameraComponent>()) {
      out << YAML::Key << "CameraComponent";
      out << YAML::BeginMap;// CameraComponent

      auto& cameraComponent     = entity.GetComponent<CameraComponent>();
      const SceneCamera& camera = cameraComponent.Camera;
      out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary
          << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
      out << YAML::Key << "Camera" << YAML::Value
          << YAML::BeginMap// Camera
          << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType()
          << YAML::Key << "FOV" << YAML::Value << camera.GetPerspectiveVerticalFOV()
          << YAML::Key << "PerspectiveNearClip" << YAML::Value << camera.GetPerspectiveNearClip()
          << YAML::Key << "PerspectiveFarClip" << YAML::Value << camera.GetPerspectiveFarClip()
          << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize()
          << YAML::Key << "OrthographicNearClip" << YAML::Value << camera.GetOrthographicNearClip()
          << YAML::Key << "OrthographicFarClip" << YAML::Value << camera.GetOrthographicFarClip()
          << YAML::EndMap;// Camera
      out << YAML::EndMap;// CameraComponent
    }
    out << YAML::EndMap;// Entity
  }

}// namespace AGE
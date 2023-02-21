//
// Created by alex on 2/21/23.
//

#pragma once

#include <yaml-cpp/yaml.h>

#include "Age/Core/Core.h"
#include "Entity.h"
#include "Scene.h"

namespace AGE {
  class SceneSerializer {
  public:
    SceneSerializer(const Ref<Scene>& scene);

    void Serialize(const age_string_t& filepath);
    void SerializeRuntime(const age_string_t& filepath);

    bool Deserialize(const age_string_t& filepath);
    bool DeserializeRuntime(const age_string_t& filepath);

    static void SerializeEntity(YAML::Emitter& out, Entity& entity);

  private:
    Ref<Scene> m_Scene;
  };

}// namespace AGE
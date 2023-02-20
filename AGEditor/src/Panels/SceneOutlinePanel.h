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
    void DrawEntityNode(Entity& entity);

  private:
    Ref<Scene> m_Context;
    Entity m_SelectedEntity;

    friend class Scene;
  };

}// namespace AGE

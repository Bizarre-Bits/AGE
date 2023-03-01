//
// Created by alex on 2/5/23.
//

#ifndef AGE_EDITORLAYER_H
#define AGE_EDITORLAYER_H

#include <Age/Age.h>
#include <Panels/SceneOutlinePanel.h>

#include <ImGuizmo.h>

namespace AGE {

  class EditorLayer : public Layer {
  public:
    EditorLayer();
    virtual void OnUpdate(Timestep ts) override;
    virtual void OnUiRender(Timestep ts) override;
    virtual void OnEvent(Event& e) override;
    virtual void OnAttach() override;

  private:
    void SetDarkThemeColors();

    void CreateNewScene();
    void SaveSceneAsDialog();
    void SaveScene();
    void OpenSceneDialog();

    void MainMenuBar();
    void Viewport();

    bool OnKeyPressed(KeyPressedEvent& e);
    bool OnMouseClicked(MouseButtonPressedEvent& e);

  private:
    bool m_ViewportHovered;
    bool m_ViewportFocused;
    glm::vec2 m_ViewportMousePos;
    glm::vec2 m_ViewportSize;
    glm::vec2 m_ViewportBounds[2];

    Ref <Framebuffer> m_Framebuffer;
    Ref <Scene> m_ActiveScene;
    EditorCamera m_EditorCamera;

    age_string_t m_SceneFilepath;
    ImGuizmo::MODE m_GizmoMode{ImGuizmo::MODE::LOCAL};
    ImGuizmo::OPERATION m_GizmoOperation{static_cast<ImGuizmo::OPERATION>(0)};

    //Panels
    SceneOutlinePanel m_SceneOutlinePanel;
  };

}// namespace AGE

#endif//AGE_EDITORLAYER_H

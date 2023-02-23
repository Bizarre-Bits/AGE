//
// Created by alex on 2/5/23.
//

#ifndef AGE_EDITORLAYER_H
#define AGE_EDITORLAYER_H

#include <Age/Age.h>
#include <Panels/SceneOutlinePanel.h>

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

    void SetupDemoFeatures();

    void MainMenuBar();
    void Viewport();

    bool OnKeyPressed(KeyPressedEvent& e);

  private:
    glm::vec2 m_ViewportSize;
    Ref<Framebuffer> m_Framebuffer;
    Ref<Scene> m_ActiveScene;
    age_string_t m_SceneFilepath;
    OrthographicCameraController m_ViewportCameraController;

    bool m_IsViewportHovered{false};
    bool m_IsViewportFocused{false};

    Entity m_RedSquareEntity;
    Entity m_BlueSquareEntity;
    Entity m_CameraA;

    glm::vec4 m_BgColor{0.05f, 0.05f, 0.05f, 1.0f};

    //Panels
    SceneOutlinePanel m_SceneOutlinePanel;
  };

}// namespace AGE

#endif//AGE_EDITORLAYER_H

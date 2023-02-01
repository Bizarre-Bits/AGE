//
// Created by alex on 20.10.22.
//

#ifndef AGE_SANDBOX2DLAYER_H
#define AGE_SANDBOX2DLAYER_H

#include "Age/Age.h"
#include "Particle.h"

class Sandbox2DUI;

class Sandbox2DLayer : public AGE::Layer {
public:
  Sandbox2DLayer();

  virtual void OnUpdate(AGE::Timestep ts) override;
  virtual void OnEvent(AGE::Event& e) override;

private:
  AGE::OrthographicCameraController m_CameraController;
  AGE::Ref<AGE::Texture2D>          m_SpriteSheet;
  std::unordered_map<char, AGE::Ref<AGE::SubTexture2D>> m_TileMap;

  friend class Sandbox2DUI;
};

class Sandbox2DUI : public AGE::ImGuiLayer {
public:
  virtual void OnUpdate(AGE::Timestep ts) override;
  virtual void OnAttach() override;

private:
  glm::vec3 m_BgColor{0.2f, 0.2f, 0.2f};
};


#endif //AGE_SANDBOX2DLAYER_H

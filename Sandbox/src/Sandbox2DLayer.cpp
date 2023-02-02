//
// Created by alex on 20.10.22.
//

#include "agepch.h"

#include "Sandbox2DLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Age/Age.h"

#include "glm/gtc/type_ptr.hpp"

static const char* s_MapTiles =
                         "WWWWWWWWWWWWWWWWWWWWWWWW"
                         "WWWWWWDDDDDDDDDDDWWWWWWW"
                         "WWWWDDDDDDDDDDDDDDWWWWWW"
                         "WWWWDDDDDDDDDDDDDDDWWWWW"
                         "WWWDDDDDDDWWWWDDDDDDWWWW"
                         "WWDDDDDWWWWWWWDDDDDDWWWW"
                         "WWDDDDDWWWWWSDDDDDDDDWWW"
                         "WWWDDDDWWWWDDDDDDDDDDWWW"
                         "WWWDDDDDDDDDDDDDDDDDWWWW"
                         "WWWWDDDDDDDDDDDDDDDWWWWW"
                         "WWWWWWWWDDDDDDDDDWWWWWWW"
                         "WWWWWWWWWWWWWWWWWWWWWWWW";

Sandbox2DLayer::Sandbox2DLayer() : m_CameraController(1280.0f / 720.0f), m_Particles(5000) {
  AGE_PROFILE_FUNCTION();

  m_SpriteSheet = AGE::Texture2D::Create("assets/textures/tilemap.png");

  m_TileMap['W'] = AGE::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11.0f, 1.0f}, {128.0f, 128.0f});
  m_TileMap['D'] = AGE::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6.0f, 1.0f}, {128.0f, 128.0f});

  m_CameraController.SetZoomLevel(7.0f);
}

void Sandbox2DLayer::OnUpdate(AGE::Timestep ts) {
  AGE_PROFILE_FUNCTION();

  m_CameraController.OnUpdate(ts);

  AGE::RenderCommand::Clear();
  AGE::Renderer2D::BeginScene(m_CameraController.GetCamera());

  uint32_t mapHeight{12};
  uint32_t mapWidth{24};
  for(uint32_t y{0}; y < mapHeight; y++) {
    float normY = (float)y - ((float)mapHeight / 2.0f);
    for(uint32_t x{0}; x < mapWidth; x++) {
      float normX = (float)x - ((float)mapWidth / 2.0f);
      if(m_TileMap.find(s_MapTiles[y * mapWidth +  x]) == m_TileMap.end()) {
        AGE::Renderer2D::DrawQuad({normX, normY, -0.1f}, {1.0f, 1.0f}, AGE::Texture2D::ErrorTexture());
        continue;
      }
      AGE::Renderer2D::DrawQuad({normX, normY, -0.1f}, {1.0f, 1.0f}, m_TileMap[s_MapTiles[y * mapWidth +  x]]);
    }
  }

  static float lastParticleSpawn{0.0f};

  static ParticleProps props{glm::vec2{0.0f},
                             glm::vec2{0},
                             1.5f,
                             45.0f,
                             90.0f,
                             glm::vec4{2.5f, 2.0f, 1.0f, 1.0f},
                             glm::vec4{0.6f, 0.0f, 0.0f, 0.3f},
                             0.05f,
                             0.1f,
                             0.05f,
                             0.01,
                             1.0f,
                             0.0f};

  lastParticleSpawn += ts;

  if(AGE::Input::IsMousePressed(AGE::Mouse::ButtonLeft)) {
    auto [x,y] = AGE::Input::MousePos();
    auto windowWidth = AGE::Application::Instance()->Window()->Width();
    auto windowHeight = AGE::Application::Instance()->Window()->Height();

    auto bounds = m_CameraController.Bounds();
    auto cameraPos = m_CameraController.GetCamera().Position();
    x = ((float)x/(float)windowWidth) * bounds.Width() - bounds.Width() * 0.5f;
    y = bounds.Height() * 0.5f - ((float)y/(float)windowHeight) * bounds.Height();

    props.Position = {x,y};
    for(int i{0}; i < 10; i++) {
      m_Particles.Emit(props);
    }
  }

  m_Particles.OnUpdate(ts);
  m_Particles.Render();

  AGE::Renderer2D::EndScene();
}

void Sandbox2DLayer::OnEvent(AGE::Event& e) {
  AGE_PROFILE_FUNCTION();

  m_CameraController.OnEvent(e);
}

void Sandbox2DUI::OnAttach() {
  AGE_PROFILE_FUNCTION();

  ImGuiLayer::OnAttach();

  AGE::RenderCommand::SetClearColor({m_BgColor, 1.0f});
}

void Sandbox2DUI::OnUpdate(AGE::Timestep ts) {
  AGE_PROFILE_FUNCTION();

  ImGui::Begin("Background");
  if (ImGui::ColorEdit3("Color", glm::value_ptr(m_BgColor))) {
    AGE::RenderCommand::SetClearColor(glm::vec4(m_BgColor, 1.0f));
  }
  ImGui::End();
}

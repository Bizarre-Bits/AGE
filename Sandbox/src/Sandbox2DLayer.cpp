//
// Created by alex on 20.10.22.
//

#include "agepch.h"

#include "Sandbox2DLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Age/Age.h"

#include "glm/gtc/type_ptr.hpp"

Sandbox2DLayer::Sandbox2DLayer() : m_CameraController(1280.0f / 720.0f), m_Particles(5000) {
  AGE_PROFILE_FUNCTION();

  m_CreeperFaceTex  = AGE::Texture2D::Create("assets/textures/creeper-face.png");
  m_IncorrectTex    = AGE::Texture2D::Create("wrong/path/to.png");
  m_CheckerboardTex = AGE::Texture2D::Create("assets/textures/Checkerboard.png");
  m_GearTex         = AGE::Texture2D::Create("assets/textures/gear-sprite.png");
}

void Sandbox2DLayer::OnUpdate(AGE::Timestep ts) {
  AGE_PROFILE_FUNCTION();

  m_CameraController.OnUpdate(ts);

  AGE::RenderCommand::Clear();
  AGE::Renderer2D::BeginScene(m_CameraController.GetCamera());

  AGE::Renderer2D::DrawQuad(glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec2{20.0f}, m_CheckerboardTex, 20.0f);

  int xCount{5};
  int yCount{xCount};

  for (int x{-xCount}; x <= xCount; x++) {
    float    normX = (float)(x + xCount) / ((float)xCount * 2.0f);
    for (int y{-yCount}; y <= yCount; y++) {
      float normY = (float)(y + yCount) / ((float)yCount * 2.0f);

      float r = normX * 0.6f;
      float b = normY * 0.6f;
      float g = 0.4f;

      AGE::Renderer2D::DrawQuad(glm::vec3{x, y, -0.1f}, glm::vec2{0.9f}, m_CheckerboardTex, glm::vec4{r, g, b, 1.0f}, 0.25f);
    }
  }

  static float lastParticleSpawn{0.0f};

  static ParticleProps props{glm::vec2{0.0f},
                             glm::vec2{0, 1.5f},
                             0.5f,
                             45.0f,
                             90.0f,
                             glm::vec4{0.9f, 0.6f, 0.0f, 1.0f},
                             glm::vec4{0.6f, 0.1f, 0.0f, 1.0f},
                             0.05f,
                             0.1f,
                             0.05f,
                             0.01,
                             5.0f,
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

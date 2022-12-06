//
// Created by alex on 20.10.22.
//

#include "agepch.h"

#include "Sandbox2DLayer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Age/Age.h"

#include "glm/gtc/type_ptr.hpp"

Sandbox2DLayer::Sandbox2DLayer() : m_CameraController(1280.0f / 720.0f) {
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

  int sq{2};

  static float globalRotationDeg{0.0f};
  globalRotationDeg += 45.0f * ts;

  glm::mat4 transform = glm::rotate(
      glm::mat4(1.0f), glm::radians(globalRotationDeg), glm::vec3{0.0f, 0.0f, 1.0f}
  );

  struct posRot {
    glm::vec3 pos;
    float     rot{0};
  };

  int    posLen = (sq * 2 + 1) * (sq * 2 + 1);
  posRot posArr[posLen];
  posRot* posPtr{posArr};

  for (int x{-sq}; x <= sq; x++) {
    for (int y{-sq}; y <= sq; y++) {
      static float time{0};
      time += ts;

      float clampx{(float)x / (float)sq};
      float clampy{(float)y / (float)sq};

      if (x == 0 && y == 0) {
        continue;
      }


      glm::vec4 modelPos{glm::vec3{
          (float)x * 0.40f,
          (float)y * 0.40f,
          ((float)(x * y)) / 10.0f},
                         1.0f};
      glm::vec4 transformedPos{transform * modelPos};
      glm::vec3 pos{transformedPos.x, transformedPos.y, transformedPos.z};

      posPtr->pos = pos;
      posPtr->rot = clampx * clampy * 10.0f * time + globalRotationDeg;
      posPtr++;
    }
  }

  std::sort(
      posArr, posArr
              + posLen, [](
          posRot a, posRot
      b
      ) {
        return a.pos.z < b.pos.
            z;
      }
  );

  for (
      auto it{posArr};
      it < posArr +
           posLen;
      it++) {
    AGE::Renderer2D::DrawRotatedQuad(
        it->pos,
        glm::vec2{0.40f},
        it->rot,
        m_GearTex,
        glm::vec4{1.0f},
        1.0f
    );
  }

  AGE::Renderer2D::DrawQuad(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec2{0.4f}, m_GearTex);

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

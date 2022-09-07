//
// Created by a on 24.08.22.
//

#include "Age/Age.h"

#include "imgui.h"

class ImGuiExample : public AGE::ImGuiLayer {
public:
  virtual void OnUpdate() override {
    Begin();
    ImGui::Text("Hello");
    End();
  }
};

class Sandbox : public AGE::Application {
public:
  Sandbox() {
    PushOverlay(new ImGuiExample);
  }

  ~Sandbox() {}
};

// Define app factory for entry point to have something to start.
AGE::Application* AGE::CreateApplication() {
  return new Sandbox{};
}
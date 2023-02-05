#include "Age/EntryPoint.h"
#include "Age/Age.h"

namespace Sandbox {
  class SandboxLayer : public AGE::Layer {
      public:
      virtual void OnUpdate(AGE::Timestep ts) override {
        AGE::RenderCommand::Clear();
      }
  };
} // Sandbox

AGE::Application* AGE::CreateApplication() {
    AGE::Application* app = new AGE::Application{};
    app->PushLayer(new Sandbox::SandboxLayer());
    return app;
}

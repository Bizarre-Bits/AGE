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
    AGE::ApplicationSpecs specs{};
    specs.AppTitle = "Sandbox";
    AGE::Application* app = new AGE::Application{specs};
    app->PushLayer(new Sandbox::SandboxLayer());
    return app;
}

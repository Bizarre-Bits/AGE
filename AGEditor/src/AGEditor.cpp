#include "Age/EntryPoint.h"
#include "Age/Age.h"

namespace AGEd {
  class AGEditorLayer : public AGE::Layer {
      public:
      virtual void OnUpdate(AGE::Timestep ts) override {
        AGE::RenderCommand::Clear();
      }
  };
} // AGEd

AGE::Application* AGE::CreateApplication() {
    AGE::Application* app = new AGE::Application{};
    app->PushLayer(new AGEd::AGEditorLayer());
    return app;
}

#include "Age/EntryPoint.h"
#include "Age/Age.h"

namespace VkE {
  class VulkanExperimentLayer : public AGE::Layer {
      public:
      virtual void OnUpdate(AGE::Timestep ts) override {
        AGE::RenderCommand::Clear();
      }
  };
} // VkE

AGE::Application* AGE::CreateApplication() {
    AGE::ApplicationSpecs specs{};
    specs.AppTitle = "VulkanExperiment";
    AGE::Application* app = new AGE::Application{specs};
    app->PushLayer(new VkE::VulkanExperimentLayer());
    return app;
}

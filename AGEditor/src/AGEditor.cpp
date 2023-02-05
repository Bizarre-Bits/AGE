#include "Age/EntryPoint.h"
#include "Editor.h"

AGE::Application* AGE::CreateApplication() {
  AGE::ApplicationSpecs specs{};
  specs.AppTitle = "AGEditor";
  auto* app = new AGE::Editor{specs};
  return app;
}

//
// Created by a on 24.08.22.
//

#ifndef AGE_ENTRYPOINT_H
#define AGE_ENTRYPOINT_H

#include "Age/Core/Application.h"
#include "Age/Core/Core.h"
#include "Age/Core/Log.h"
#include "Age/Debug/Instrumentor.h"
#include "Age/Utils/Random.h"

extern AGE::Application* AGE::CreateApplication();

int main(int argc, char** argv) {
  bool globalProfile{false};

  for (int i{1}; i < argc; i++) {
    if (strcmp(argv[i], "--profile") == 0) {
      globalProfile = true;
    }
  }

  if (globalProfile)
    AGE_PROFILE_BEGIN_SESSION("Startup", "age_profile_startup.json");

  AGE::Log::Init();
  AGE_CORE_TRACE("Initialized logging");

  auto app = AGE::CreateApplication();
  if (globalProfile)
    AGE_PROFILE_END_SESSION();

  if (globalProfile)
    AGE_PROFILE_BEGIN_SESSION("Runtime", "age_profile_runtime.json");
  app->Run();
  if (globalProfile)
    AGE_PROFILE_END_SESSION();
  if (globalProfile)
    AGE_PROFILE_BEGIN_SESSION("Shutdown", "age_profile_shutdown.json");
  delete app;
  AGE_CORE_TRACE("App is terminated");
  if (globalProfile)
    AGE_PROFILE_END_SESSION();
}

#endif//AGE_ENTRYPOINT_H

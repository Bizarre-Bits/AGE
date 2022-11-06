//
// Created by a on 24.08.22.
//

#ifndef AGE_ENTRYPOINT_H
#define AGE_ENTRYPOINT_H

#include "Age/Core/Core.h"
#include "Age/Core/Log.h"
#include "Age/Core/Application.h"
#include "Age/Debug/Instrumentor.h"

extern AGE::Application* AGE::CreateApplication();

int main(int argc, char** argv) {
  AGE_PROFILE_BEGIN_SESSION("Startup", "age_profile_startup.json");

  AGE::Log::Init();
  AGE_CORE_TRACE("Initialized logging");

  auto app = AGE::CreateApplication();
  AGE_PROFILE_END_SESSION();

  AGE_PROFILE_BEGIN_SESSION("Runtime", "age_profile_runtime.json");
  app->Run();
  AGE_PROFILE_END_SESSION();

  AGE_PROFILE_BEGIN_SESSION("Shutdown", "age_profile_shutdown.json");
  delete app;
  AGE_CORE_TRACE("App is terminated");
  AGE_PROFILE_END_SESSION();
}

#endif//AGE_ENTRYPOINT_H

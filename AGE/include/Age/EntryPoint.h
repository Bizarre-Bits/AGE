//
// Created by a on 24.08.22.
//

#ifndef AGE_ENTRYPOINT_H
#define AGE_ENTRYPOINT_H

#include "Age/Core/Core.h"
#include "Age/Core/Log.h"
#include "Age/Core/Application.h"

extern AGE::Application* AGE::CreateApplication();

int main(int argc, char** argv) {
  AGE::Log::Init();
  AGE_CORE_TRACE("Initialized logging");

  auto app = AGE::CreateApplication();

  app->Run();
  delete app;

  AGE_CORE_TRACE("App is terminated");
}

#endif//AGE_ENTRYPOINT_H

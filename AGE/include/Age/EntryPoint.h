//
// Created by a on 24.08.22.
//

#ifndef AGE_ENTRYPOINT_H
#define AGE_ENTRYPOINT_H

#include "Core.h"

#include "Events/WindowEvent.h"

extern AGE::Application* AGE::CreateApplication();

int main (int argc, char** argv) {
	AGE::Log::Init();

	CORE_LOG_DEBUG("Initialized logging");
	auto app = AGE::CreateApplication();

	AGE::WindowCloseEvent event{};
	CORE_LOG_INFO(event);

	CORE_LOG_DEBUG("Created app");

	app->Run();
	delete app;

	CORE_LOG_DEBUG("App is terminated");

}

#endif //AGE_ENTRYPOINT_H

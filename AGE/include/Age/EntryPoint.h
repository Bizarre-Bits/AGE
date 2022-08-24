//
// Created by a on 24.08.22.
//

#ifndef AGE_ENTRYPOINT_H
#define AGE_ENTRYPOINT_H

#include "Core.h"

extern AGE::Application* AGE::CreateApplication();

int main (int argc, char** argv) {
	auto app = AGE::CreateApplication();
	app->Run();
	delete app;
}

#endif //AGE_ENTRYPOINT_H

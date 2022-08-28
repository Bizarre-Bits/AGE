//
// Created by a on 24.08.22.
//

#include "agepch.h"

#include "Age/Application.h"

namespace AGE {
	Application::Application() {
		window_ = Window::Create();
	}

	Application::~Application() {
		delete window_;
	}

	void Application::Run() {
		while(running_) {
			window_->OnUpdate();
		}
	}
} // Age
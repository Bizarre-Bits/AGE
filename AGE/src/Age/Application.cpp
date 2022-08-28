//
// Created by a on 24.08.22.
//

#include "agepch.h"

#include "Age/Application.h"

namespace AGE {
	Application::Application() {
		window_ = Window::Create();
		window_->EventCallback(AGE_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application() {
		delete window_;
	}

	void Application::Run() {
		while(running_) {
			window_->OnUpdate();
		}
	}
	void Application::OnEvent(Event &e) {
		AGE_CORE_INFO(e);
		if(e.GetEventType() == EventType::WindowClose) {
			running_ = false;
		}
	}
} // Age
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
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(AGE_BIND_EVENT_FN(Application::OnWindowClose));

		AGE_CORE_TRACE(e);
	}
	bool Application::OnWindowClose(WindowCloseEvent &e) {
		running_ = false;
		return true;
	}
} // Age
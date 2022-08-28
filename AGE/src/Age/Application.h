//
// Created by a on 24.08.22.
//

#ifndef AGE_APPLICATION_H
#define AGE_APPLICATION_H

#include "Age/Core.h"

#include "Window.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"

namespace AGE {
	class DLL_PUBLIC Application {
		public:
		Application();
		virtual ~Application();

		virtual void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		Window* window_;
		bool running_{true};
	};

	// To be defined in CLIENT
	Application* CreateApplication();
} // Age

#endif //AGE_APPLICATION_H

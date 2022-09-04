//
// Created by a on 24.08.22.
//

#ifndef AGE_APPLICATION_H
#define AGE_APPLICATION_H

#include "Age/Core.h"

#include "Age/Window.h"
#include "Age/Events/Event.h"
#include "Age/Events/WindowEvent.h"
#include "Age/LayerStack.h"

namespace AGE {
	class DLL_PUBLIC Application {
		public:
		Application();
		virtual ~Application();

		virtual void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		Window* window_;
		bool running_{true};
		LayerStack layerStack_;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
} // Age

#endif //AGE_APPLICATION_H

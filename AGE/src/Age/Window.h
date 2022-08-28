//
// Created by alex on 28.08.22.
//

#ifndef AGE_WINDOW_H
#define AGE_WINDOW_H

#include "agepch.h"

#include "Core.h"
#include "Events/Event.h"

namespace AGE {
	struct WindowProps {
		age_string_t title;
		unsigned int width, height;

		explicit WindowProps(const age_string_t &title = "AGEngine", unsigned int width = 1280, unsigned int height = 720) :
						title{title}, width{width}, height{height} {}
	};

	/**
	 * @brief Interface representing a desktop based system's window
	 */
	class DLL_PUBLIC Window {
	public:
		using EventCallbackFn = std::function<void(Event &)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int Width() const = 0;
		virtual unsigned int Height() const = 0;

		virtual void EventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		// Must be implemented per Platform
		static Window* Create(const WindowProps& props = WindowProps());
	};
}

#endif //AGE_WINDOW_H

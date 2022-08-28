//
// Created by alex on 28.08.22.
//

#ifndef AGE_OPENGLWINDOW_H
#define AGE_OPENGLWINDOW_H

#include "Age/Core.h"
#include "Age/Window.h"

#include "GLFW/glfw3.h"

namespace AGE {
	class DLL_PUBLIC OpenGLWindow : public Window {
	public:
		OpenGLWindow(const WindowProps &props);
		virtual ~OpenGLWindow();

		virtual void OnUpdate() override;

		inline unsigned int Width() const override { return data_.width; }
		inline unsigned int Height() const override { return data_.height; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void EventCallback(const EventCallbackFn &callback) override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Destroy();

	private:
		GLFWwindow *window_;

		struct WindowData {
			age_string_t title;
			unsigned int width, height;
			bool vSync;
		} data_;

		static bool s_GLFWInitialized;
	};
} // AGE

#endif //AGE_OPENGLWINDOW_H

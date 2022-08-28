//
// Created by alex on 28.08.22.
//

#include "agepch.h"

#include "OpenGLWindow.h"

#include "Age/Assert.h"

namespace AGE {
	bool OpenGLWindow::s_GLFWInitialized{false};

	void GLFWErrorCallback(int error, const char *description) {
		AGE_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window *Window::Create(const AGE::WindowProps &props) {
		return new OpenGLWindow(props);
	}

	OpenGLWindow::OpenGLWindow(const WindowProps &props) {
		Init(props);
	}

	OpenGLWindow::~OpenGLWindow() {
		Destroy();
	}

	void OpenGLWindow::Init(const WindowProps &props) {
		data_.title = props.title;
		data_.width = props.width;
		data_.height = props.height;

		AGE_CORE_INFO("Creating Window \"{0}\" ({1}, {2})", props.title, props.width, props.height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			AGE_CORE_ASSERT(success, "Could not initialize GLFW");
			s_GLFWInitialized = true;
			AGE_CORE_INFO("Initialized GLFW: {0}", glfwGetVersionString());
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		window_ = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
		glfwSetWindowUserPointer(window_, &data_);
		glfwMakeContextCurrent(window_);
		SetVSync(true);

		AGE_CORE_INFO("Created Window \"{0}\" ({1}, {2})", props.title, props.width, props.height);
	}

	void OpenGLWindow::OnUpdate() {
		glfwPollEvents();
		glfwSwapBuffers(window_);
	}
	void OpenGLWindow::SetVSync(bool enabled) {
		if (enabled) {
			glfwSwapInterval(1);
		} else {
			glfwSwapInterval(0);
		}
		data_.vSync = enabled;
	}
	bool OpenGLWindow::IsVSync() const {
		return data_.vSync;
	}
	void OpenGLWindow::EventCallback(const Window::EventCallbackFn &callback) {

	}
	void OpenGLWindow::Destroy() {
		glfwDestroyWindow(window_);
	}
} // AGE
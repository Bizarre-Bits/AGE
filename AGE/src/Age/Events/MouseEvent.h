//
// Created by alex on 27.08.22.
//

#ifndef AGE_MOUSEEVENT_H
#define AGE_MOUSEEVENT_H

#include "Event.h"

#include "../MouseCodes.h"

namespace AGE {
	class DLL_PUBLIC MouseButtonEvent : public Event {
	public:

		inline MouseCode MouseButton() const { return mouseButtonCode_; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

	protected:
		explicit MouseButtonEvent(MouseCode mouseButtonCode) : mouseButtonCode_(mouseButtonCode) {}

		MouseCode mouseButtonCode_;
	};

	class DLL_PUBLIC MouseButtonPressedEvent : public MouseButtonEvent {
	public:
		explicit MouseButtonPressedEvent(MouseCode mouseButtonCode) : MouseButtonEvent(mouseButtonCode) {}

		virtual const age_string_t ToString() const override {
			std::stringstream ss;
			ss << Name() << ": " << mouseButtonCode_;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class DLL_PUBLIC MouseButtonReleasedEvent : public MouseButtonEvent {
	public:
		explicit MouseButtonReleasedEvent(MouseCode mouseButtonCode) : MouseButtonEvent(mouseButtonCode) {}

		virtual const age_string_t ToString() const override {
			std::stringstream ss;
			ss << Name() << ": " << mouseButtonCode_;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class DLL_PUBLIC MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) : x_(x), y_(y) {}

		inline float X() const { return x_; }

		inline float Y() const { return y_; }

		virtual const age_string_t ToString() const override {
			std::stringstream ss;
			ss << Name() << ": " << x_ << ", " << y_;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

	private:
		float x_, y_;
	};

	class DLL_PUBLIC MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset) : xOffset_(xOffset), yOffset_(yOffset) {}

		inline float XOffset() const { return xOffset_; }

		inline float YOffset() const { return yOffset_; }

		virtual const age_string_t ToString() const override {
			std::stringstream ss;
			ss << Name() << ": " << xOffset_ << ", " << yOffset_;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

	private:
		float xOffset_, yOffset_;
	};
}

#endif //AGE_MOUSEEVENT_H

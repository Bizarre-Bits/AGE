//
// Created by a on 25.08.22.
//

#ifndef AGE_KEYEVENT_H
#define AGE_KEYEVENT_H

#include "Age/Events/Event.h"

#include "Age/KeyCodes.h"

namespace AGE {

	class DLL_PUBLIC KeyEvent : public Event {
	public:

		inline KeyCode GetKeyCode() const { return keycode_; }

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

	protected:
		explicit KeyEvent(const KeyCode keycode) : keycode_{keycode} {}

		KeyCode keycode_;
	};

	class DLL_PUBLIC KeyPressedEvent : public KeyEvent {
	public:
		explicit KeyPressedEvent(const KeyCode keyCode, const bool isRepeated = false) : KeyEvent(keyCode),
		                                                                          isRepeated_{isRepeated} {}

		virtual const age_string_t ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent: " << keycode_ << " (repeat=" << isRepeated_ << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:
		bool isRepeated_;
	};

	class DLL_PUBLIC KeyReleasedEvent : public KeyEvent {
	public:
		explicit KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

		virtual const age_string_t ToString() const override {
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << keycode_;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

} // AGE

#endif //AGE_KEYEVENT_H

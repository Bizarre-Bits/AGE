//
// Created by a on 25.08.22.
//

#ifndef AGE_KEYEVENT_H
#define AGE_KEYEVENT_H

#include "Event.h"

#include "Age/Core/KeyCodes.h"

namespace AGE {

  class DLL_PUBLIC KeyEvent : public Event {
  public:
    inline KeyCode KeyCode() const { return m_Keycode; }

    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

  protected:
    explicit KeyEvent(const ::AGE::KeyCode keycode) : m_Keycode{keycode} {}

    ::AGE::KeyCode m_Keycode;
  };

  class DLL_PUBLIC KeyPressedEvent : public KeyEvent {
  public:
    explicit KeyPressedEvent(const ::AGE::KeyCode keyCode, const bool isRepeated = false) : KeyEvent(keyCode),
                                                                                            m_IsRepeated{isRepeated} {}

    virtual const age_string_t ToString() const override {
      std::stringstream ss;
      ss << "KeyPressedEvent: " << m_Keycode << " (repeat=" << m_IsRepeated << ")";
      return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)

  private:
    bool m_IsRepeated;
  };

  class DLL_PUBLIC KeyReleasedEvent : public KeyEvent {
  public:
    explicit KeyReleasedEvent(const ::AGE::KeyCode keycode) : KeyEvent(keycode) {}

    virtual const age_string_t ToString() const override {
      std::stringstream ss;
      ss << "KeyReleasedEvent: " << m_Keycode;
      return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
  };

  class DLL_PUBLIC KeyTypedEvent : public KeyEvent {
  public:
    explicit KeyTypedEvent(const ::AGE::KeyCode keycode) : KeyEvent(keycode) {}

    const age_string_t ToString() const override {
      std::stringstream ss;
      ss << "KeyTypedEvent: " << m_Keycode;
      return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
  };

}// namespace AGE

#endif//AGE_KEYEVENT_H

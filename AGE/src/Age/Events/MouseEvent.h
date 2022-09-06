//
// Created by alex on 27.08.22.
//

#ifndef AGE_MOUSEEVENT_H
#define AGE_MOUSEEVENT_H

#include "Event.h"

#include "Age/Core/MouseCodes.h"

namespace AGE {
  class DLL_PUBLIC MouseButtonEvent : public Event {
  public:
    inline MouseCode MouseButton() const { return m_MouseButtonCode; }

    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

  protected:
    explicit MouseButtonEvent(MouseCode mouseButtonCode) : m_MouseButtonCode(mouseButtonCode) {}

    MouseCode m_MouseButtonCode;
  };

  class DLL_PUBLIC MouseButtonPressedEvent : public MouseButtonEvent {
  public:
    explicit MouseButtonPressedEvent(MouseCode mouseButtonCode) : MouseButtonEvent(mouseButtonCode) {}

    virtual const age_string_t ToString() const override {
      std::stringstream ss;
      //TODO figure out something about uint8 type
      ss << Name() << ": " << (int)m_MouseButtonCode;
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
  };

  class DLL_PUBLIC MouseButtonReleasedEvent : public MouseButtonEvent {
  public:
    explicit MouseButtonReleasedEvent(MouseCode mouseButtonCode) : MouseButtonEvent(mouseButtonCode) {}

    virtual const age_string_t ToString() const override {
      std::stringstream ss;
      //TODO figure out something about uint8 type
      ss << Name() << ": " << (int)m_MouseButtonCode;
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
  };

  class DLL_PUBLIC MouseMovedEvent : public Event {
  public:
    MouseMovedEvent(float x, float y) : m_X(x), m_Y(y) {}

    inline float X() const { return m_X; }
    inline float Y() const { return m_Y; }

    virtual const age_string_t ToString() const override {
      std::stringstream ss;
      ss << Name() << ": " << m_X << ", " << m_Y;
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

  private:
    float m_X, m_Y;
  };

  class DLL_PUBLIC MouseScrolledEvent : public Event {
  public:
    MouseScrolledEvent(const float xOffset, const float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

    inline float XOffset() const { return m_XOffset; }
    inline float YOffset() const { return m_YOffset; }

    virtual const age_string_t ToString() const override {
      std::stringstream ss;
      ss << Name() << ": " << m_XOffset << ", " << m_YOffset;
      return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

  private:
    float m_XOffset, m_YOffset;
  };
}// namespace AGE

#endif//AGE_MOUSEEVENT_H

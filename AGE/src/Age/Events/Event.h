//
// Created by a on 25.08.22.
//

#pragma once

#include "agepch.h"

#include "Age/Core/Core.h"

namespace AGE {

  enum class  EventType {
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    KeyPressed,
    KeyReleased,
    KeyTyped,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
  };

  enum  EventCategory {
    None                     = 0,
    EventCategoryApplication = BIT(0),
    EventCategoryInput       = BIT(1),
    EventCategoryKeyboard    = BIT(2),
    EventCategoryMouse       = BIT(3),
    EventCategoryMouseButton = BIT(4),
  };

#define EVENT_CLASS_TYPE(type)                                             \
  static enum EventType StaticType() { return EventType::type; }                \
  virtual enum EventType EventType() const override { return StaticType(); } \
  virtual const char* Name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
  virtual int CategoryFlags() const override { return category; }

  class  Event {
    friend class EventDispatcher;

  public:
    virtual enum EventType EventType() const = 0;
    virtual const char* Name() const       = 0;
    virtual int CategoryFlags() const      = 0;

    virtual const age_string_t ToString() const { return Name(); }

    inline bool IsInCategory(EventCategory category) const { return CategoryFlags() & category; }
    bool Handled{false};
  };

  class  EventDispatcher {
  public:
    EventDispatcher(Event& event) : m_Event{event} {}

    /**
		 * @brief Dispatches event
		 * @tparam E - Event type.
		 * @tparam F - dispatch callback, will be deduced by compiler.
		 * @param func - dispatch callback.
		 * @return <tt>true</tt> if event type is correct and event successfully dispatched; <tt>false</tt> if event is
		 * undispatchable for it's incorrect type.
		 */
    template<typename E, typename F>
    bool Dispatch(const F& func) {
      if (m_Event.EventType() == E::StaticType()) {
        m_Event.Handled |= func(static_cast<E&>(m_Event));
        return true;
      }

      return false;
    }

  private:
    Event& m_Event;
  };

   inline std::ostream& operator<<(std::ostream& os, const Event& e) {
    return os << e.ToString();
  }

}// namespace AGE

//
// Created by alex on 27.08.22.
//

#pragma once

#include "Event.h"

namespace AGE {
  class  WindowResizeEvent : public Event {
  public:
    WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)

    virtual const age_string_t ToString() const override {
      std::stringstream ss;
      ss << Name() << ": " << m_Width << ", " << m_Height;
      return ss.str();
    }

    inline unsigned int Width() const { return m_Width; }
    inline unsigned int Height() const { return m_Height; }

  private:
    unsigned int m_Width, m_Height;
  };

  class  WindowCloseEvent : public Event {
  public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
  };
}// namespace AGE

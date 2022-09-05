//
// Created by alex on 27.08.22.
//

#ifndef AGE_WINDOWEVENT_H
#define AGE_WINDOWEVENT_H

#include "Event.h"

namespace AGE {
    class DLL_PUBLIC WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : width_(width), height_(height) {}

        EVENT_CLASS_TYPE(WindowResize)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)

        virtual const age_string_t ToString() const override {
            std::stringstream ss;
            ss << Name() << ": " << width_ << ", " << height_;
            return ss.str();
        }

        inline unsigned int Width() const { return width_; }

        inline unsigned int Height() const { return height_; }

    private:
        unsigned int width_, height_;
    };

    class DLL_PUBLIC WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)

        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}

#endif //AGE_WINDOWEVENT_H

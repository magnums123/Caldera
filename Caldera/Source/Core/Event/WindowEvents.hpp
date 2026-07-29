#pragma once

#include <Defines.hpp>

#include "Core/Event/Event.hpp"

namespace CAL
{

class WindowCloseEvent : public Event
{
   public:
    WindowCloseEvent() : Event(EventType::WINDOW_CLOSED, "WindowCloseEvent") {}
    ~WindowCloseEvent() {}
};

class WindowResizeEvent : public Event
{
   public:
    WindowResizeEvent() : Event(EventType::WINDOW_RESIZED, "WindowResizeEvent"), width(0), height(0) {}
    WindowResizeEvent(uint32_t w, uint32_t h)
        : Event(EventType::WINDOW_RESIZED, "WindowResizeEvent"), width(w), height(h)
    {
    }
    ~WindowResizeEvent() {}

    inline uint32_t getWidth() const { return width; }
    inline uint32_t getHeight() const { return height; }

   private:
    uint32_t width, height;
};

}  // namespace CAL

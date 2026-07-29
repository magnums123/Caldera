#pragma once

#include <Defines.hpp>

#include "Core/Event/Event.hpp"

namespace CAL
{
enum class MouseButton
{
    Left,
    Middle,
    Right
};

class MouseMoveEvent : public Event
{
   public:
    MouseMoveEvent(int x, int y) : Event(EventType::MOUSE_MOVE, "MouseMoveEvent"), x(x), y(y) {}
    ~MouseMoveEvent() {}

    inline int getX() const { return x; }
    inline int getY() const { return y; }

   private:
    int x, y;
};

class MouseWheelEvent : public Event
{
   public:
    MouseWheelEvent(int zDelta) : Event(EventType::MOUSE_WHEEL_SCROLLED, "MouseWheelEvent"), zDelta(zDelta) {}
    ~MouseWheelEvent() {}

    inline int getZDelta() const { return zDelta; }

   private:
    int zDelta;
};

class MouseButtonPressEvent : public Event
{
   public:
    MouseButtonPressEvent(MouseButton button)
        : Event(EventType::MOUSE_BUTTON_PRESSED, "MouseButtonPressEvent"), button(button)
    {
    }
    ~MouseButtonPressEvent() {}

    inline MouseButton getButton() const { return button; }

   private:
    MouseButton button;
};

class MouseButtonReleaseEvent : public Event
{
   public:
    MouseButtonReleaseEvent(MouseButton button)
        : Event(EventType::MOUSE_BUTTON_RELEASED, "MouseButtonReleaseEvent"), button(button)
    {
    }
    ~MouseButtonReleaseEvent() {}

    inline MouseButton getButton() const { return button; }

   private:
    MouseButton button;
};

}  // namespace CAL

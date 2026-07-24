#pragma once

#include <Defines.hpp>

#include "Core/Event/Event.hpp"
#include "Core/Event/KeyCodes.hpp"

namespace CAL
{

class KeyPressEvent : public Event
{
   public:
    KeyPressEvent(KeyCodeID keyCode) : Event(EventType::KEY_PRESSED, "KeyPressEvent"), keyCode(keyCode) {}
    ~KeyPressEvent() {}

    inline KeyCodeID getKeyCode() const { return keyCode; }

   private:
    KeyCodeID keyCode{ KeyCodeID::Unknown };
};

class KeyReleaseEvent : public Event
{
   public:
    KeyReleaseEvent(KeyCodeID keyCode) : Event(EventType::KEY_RELEASED, "KeyReleaseEvent"), keyCode(keyCode) {}
    ~KeyReleaseEvent() {}

    inline KeyCodeID getKeyCode() const { return keyCode; }

   private:
    KeyCodeID keyCode{ KeyCodeID::Unknown };
};

}  // namespace CAL

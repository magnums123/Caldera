#pragma once

#include <Defines.hpp>
#include <functional>
#include <map>
#include <vector>

#include "Utility/String.hpp"

namespace CAL
{
enum class EventType
{
    WINDOW_CLOSED,
    WINDOW_RESIZED,
    KEY_PRESSED,
    KEY_RELEASED,
    MOUSE_MOVE,
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,
    MOUSE_WHEEL_SCROLLED,
};

class Event
{
   public:
    // Event() = default;
    Event(EventType type, StringView name) : type(type), name(name) {}
    virtual ~Event() {}

    bool handled{ false };

    inline EventType getType() const { return type; }
    inline StringView getName() const { return name; }

    template <class Type>
    inline Type toType() const
    {
        return dynamic_cast<Type>(this);
    }

   protected:
    EventType type;
    String name;
};

class EventDispatcher
{
   private:
    using Func = std::function<void(Event&)>;
    std::map<EventType, std::vector<Func>> listeners;

   public:
    void addListener(EventType type, const Func& func) { listeners[type].push_back(func); }
    void dispatch(Event& e)
    {
        if (listeners.find(e.getType()) == listeners.end()) return;

        for (auto& listener : listeners.at(e.getType()))
            if (!e.handled) listener(e);
    }
};

}  // namespace CAL

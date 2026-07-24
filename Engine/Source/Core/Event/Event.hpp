#pragma once

#include <Defines.hpp>
#include <functional>
#include <map>

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
    Event(EventType type, StringView name) : type(type), name(name) {}
    virtual ~Event() {}

    inline EventType getType() const { return type; }
    inline StringView getName() const { return name; }

    inline void handle() { handled = true; }
    inline bool isHandled() const { return handled; }

    template <class Type>
    inline Type toType() const
    {
        if (auto* ret = dynamic_cast<Type>(this)) return ret;
        return {};
    }

   protected:
    EventType type;
    String name;

   private:
    bool handled{ false };
};

class EventDispatcher
{
   private:
    using Func = std::function<void(Event&)>;
    std::map<EventType, std::vector<Func>> listeners;

   public:
    void addListener(EventType type, const Func& func) { listeners[type].push_back(func); }
    void dispatch(Event&& e)
    {
        if (listeners.find(e.getType()) == listeners.end()) return;

        for (auto& listener : listeners.at(e.getType()))
            if (!e.isHandled()) listener(e);
    }
};

}  // namespace CAL

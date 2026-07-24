#pragma once

#include "Core/Event/Event.hpp"
#include "Core/Memory.hpp"
#include "Defines.hpp"
#include "Utility/String.hpp"

namespace CAL
{
struct WindowCreateInfo
{
    String name;
    uint32_t width, height;
};

class Window
{
   public:
    Window(const WindowCreateInfo& createInfo) : width(createInfo.width), height(createInfo.height) {}
    virtual ~Window() = default;

    EventDispatcher dispatcher;

    inline uint32_t getWidth() const { return width; }
    inline uint32_t getHeight() const { return height; }
    inline void* getHandle() const { return handle; }

    virtual void toggleFullscreen() = 0;
    virtual bool shouldClose() = 0;
    virtual void close() = 0;
    virtual void update(float deltaTime) = 0;

    static Ref<Window> Create(const WindowCreateInfo& createInfo);

   protected:
    void* handle;
    uint32_t width, height;
};
}  // namespace CAL

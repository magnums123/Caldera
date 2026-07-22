#include "Application.h"

#include <memory>
#include <utility>

#include "Core/Event/Event.hpp"
#include "Core/Event/WindowEvents.hpp"
#include "Core/GameMemory.hpp"
#include "Core/Logger.hpp"
#include "Game/Game.hpp"

namespace CAL
{

Application::Application(const AppInfo& appInfo, std::unique_ptr<Game> game)
    : window(nullptr), gameInstance(std::move(game)), useWindow(appInfo.useWindow)
{
    Memory::initMemory();

    size_t size = 1024 * 1024;
    auto memory = Memory::allocateMemory(size, Memory::MemoryTag::APPLICATION);
    LOG_TRACE("{}", Memory::getMemoryUsageString());
    Memory::freeMemory(memory, size, Memory::MemoryTag::APPLICATION);

    if (useWindow)
    {
        WindowCreateInfo createInfo{ .name = appInfo.appName, .width = appInfo.width, .height = appInfo.height };
        window = Window::Create(createInfo);

        // TEMP
        window->dispatcher.addListener(EventType::WINDOW_CLOSED, [&](Event& e) { isRunning = false; });
        window->dispatcher.addListener(
            EventType::WINDOW_RESIZED,
            [&](Event& e)
            {
                auto re = e.toType<const WindowResizeEvent*>();
                LOG_DEBUG("Window Resized. New size = ({}, {})", re->getWidth(), re->getHeight());
            });
    }

    isRunning = true;
    isSuspended = false;
}

Application::~Application() { Memory::shutdownMemory(); }

void Application::run()
{
    while (isRunning)
    {
        if (!isSuspended)
        {
            if (useWindow)
            {
                // isRunning = !window->shouldClose();
                window->update(0);
                gameInstance->update(0);
                gameInstance->render();
            }
        }
    }
}
}  // namespace CAL

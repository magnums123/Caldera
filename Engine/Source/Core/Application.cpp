#include "Application.h"

#include "Core/Containers/Vector.hpp"
#include "Core/Event/Event.hpp"
#include "Core/Event/KeyCodes.hpp"
#include "Core/Event/KeyEvents.hpp"
#include "Core/Event/WindowEvents.hpp"
#include "Core/Logger.hpp"
#include "Core/Memory.hpp"
#include "Game/Game.hpp"

namespace CAL
{

Application::Application(const AppInfo& appInfo, Ref<Game> game)
    : window(nullptr), gameInstance(std::move(game)), useWindow(appInfo.useWindow)
{
    Memory::initMemory();

    Vector<int> vec{ { 122, 2, 3, 4, 5, 62, 7, 8, 9, 0 } };
    vec.resize(5);
    vec.pop();
    vec.pushBack(30);
    vec.remove(0);
    // for (auto i = 0; i < vec.size(); i++) LOG_DEBUG("{}", vec[i]);
    for (auto i : vec) LOG_DEBUG("{}", i);

    LOG_INFO("Size of 4 ints: {}", sizeof(int) * 4);

    LOG_TRACE("{}", Memory::getMemoryUsageString());

    if (useWindow)
    {
        WindowCreateInfo createInfo{ .name = appInfo.appName, .width = appInfo.width, .height = appInfo.height };
        window = Window::Create(createInfo);

        // TEMP
        window->dispatcher.addListener(
            EventType::WINDOW_CLOSED,
            [&](Event& e)
            {
                isRunning = false;
                e.handle();
            });
        window->dispatcher.addListener(
            EventType::KEY_PRESSED,
            [&](Event& e)
            {
                auto re = e.toType<const KeyPressEvent*>();
                auto keyCodeStr = re->getKeyCode();
                LOG_INFO("Key Pressed: {}", keyCodeToString(re->getKeyCode()));
                e.handle();
            });
        window->dispatcher.addListener(
            EventType::WINDOW_RESIZED,
            [&](Event& e)
            {
                auto re = e.toType<const WindowResizeEvent*>();

                LOG_DEBUG("Window Resized. New size = ({}, {})", re->getWidth(), re->getHeight());
                e.handle();
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

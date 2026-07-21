#include "Application.h"

#include <memory>

#include "Window/Window.hpp"

namespace CAL
{

Application::Application(const AppInfo& appInfo) : platform(Platform::Create()), window(nullptr)
{
    // platform = Platform::Create();
    if (appInfo.useWindow)
    {
        WindowCreateInfo createInfo{ .name = appInfo.appName, .width = appInfo.width, .height = appInfo.height };
        window = Window::Create(createInfo);
    }

    running = true;
}

Application::~Application() {}

void Application::run()
{
    while (running)
    {
        if (window) running = window->shouldClose();
        if (window) window->update(0);
    }
}
}  // namespace CAL

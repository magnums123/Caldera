#include "Application.h"

#include <memory>
#include <utility>

namespace CAL
{

Application::Application(const AppInfo& appInfo, std::unique_ptr<Game> game)
    : platform(Platform::Create()), window(nullptr), gameInstance(std::move(game)), useWindow(appInfo.useWindow)
{
    // platform = Platform::Create();
    if (useWindow)
    {
        WindowCreateInfo createInfo{ .name = appInfo.appName, .width = appInfo.width, .height = appInfo.height };
        window = Window::Create(createInfo);
    }

    isRunning = true;
    isSuspended = false;
}

Application::~Application() {}

void Application::run()
{
    while (isRunning)
    {
        if (!isSuspended)
        {
            if (useWindow)
            {
                isRunning = !window->shouldClose();
                window->update(0);
                gameInstance->update(0);
                gameInstance->render();
            }
        }
    }
}
}  // namespace CAL

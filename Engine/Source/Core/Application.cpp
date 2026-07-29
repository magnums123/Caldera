#include "Application.h"

#include <cstdint>

#include "Core/Clock.h"
#include "Core/Event/Event.hpp"
#include "Core/Logger.hpp"
#include "Core/Memory.hpp"
#include "Game/Game.hpp"
#include "Renderer/Renderer.hpp"

namespace CAL
{

Application::Application(const AppInfo& appInfo, Ref<Game> game)
    : window(nullptr),
      gameInstance(std::move(game)),
      platform(Platform::Create()),
      clock(nullptr),
      headless(appInfo.headless)
{
    Memory::initMemory();
    clock = CreateRef<Clock>(platform);

    if (headless)
        // Do headless configuration
        return;

    WindowCreateInfo createInfo{ .name = appInfo.appName, .width = appInfo.width, .height = appInfo.height };
    window = Window::Create(createInfo);

    RendererInfo rendererInfo{ .width = appInfo.width,
                               .height = appInfo.height,
                               .name = appInfo.appName,
                               .backendType = RendererBackendType::Vulkan,
                               .platform = platform,
                               .window = window };
    renderer = Renderer::Create(rendererInfo);

    // TEMP
    window->dispatcher.addListener(
        EventType::WINDOW_CLOSED,
        [&](Event& e)
        {
            isRunning = false;
            e.handle();
        });

    isRunning = true;
    isSuspended = false;
}

Application::~Application() { Memory::shutdownMemory(); }

void Application::run()
{
    clock->update();
    lastTime = clock->getElapsedTime();

    float runningTime{};
    uint64_t frameCount{};
    float targetFrameTime{ 1.f / 60.f };

    while (isRunning)
    {
        window->update();
        if (!isSuspended)
        {
            clock->update();
            float currentTime{ clock->getElapsedTime() };
            float deltaTime{ currentTime - lastTime };
            float frameStartTime{ platform->getAbsoluteTime() };

            gameInstance->update(deltaTime);
            gameInstance->render(deltaTime);

            RenderPacket packet{ deltaTime };
            renderer->drawFrame(packet);

            float frameEndTime{ platform->getAbsoluteTime() };
            float frameTime{ frameEndTime - frameStartTime };
            runningTime += frameTime;
            float remainingSeconds{ targetFrameTime - frameTime };

            bool limitFrames{ false };
            if (remainingSeconds > 0 && limitFrames) platform->sleep((remainingSeconds * 1000) - 1);
            frameCount += 1;

            lastTime = currentTime;
        }
    }
}
}  // namespace CAL

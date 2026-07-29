#pragma once

#include <Defines.hpp>

#include "Core/Clock.h"
#include "Game/Game.hpp"
#include "Renderer/Renderer.hpp"
#include "Utility/String.hpp"
#include "Window/Window.hpp"

namespace CAL
{

struct AppInfo
{
    String appName;
    uint32_t width, height;
    bool headless;
};

class Application
{
   public:
    Application(const AppInfo& appInfo, Ref<Game> game);
    ~Application();

    void run();

   private:
    float lastTime;
    bool isRunning{ false };
    bool isSuspended{ false };

    bool headless{ false };

    Ref<Platform> platform{ nullptr };
    Ref<Clock> clock{ nullptr };
    Ref<Window> window{ nullptr };
    Ref<Game> gameInstance{ nullptr };
    Ref<Renderer> renderer{ nullptr };
};

}  // namespace CAL

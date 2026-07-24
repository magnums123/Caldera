#pragma once

#include "Defines.hpp"
#include "Game/Game.hpp"
#include "Utility/String.hpp"
#include "Window/Window.hpp"

namespace CAL
{

struct AppInfo
{
    String appName;
    uint32_t width, height;
    bool useWindow;
};

class Application
{
   public:
    Application(const AppInfo& appInfo, Ref<Game> game);
    ~Application();

    void run();

   private:
    bool isRunning{ false };
    bool isSuspended{ false };

    bool useWindow{ true };

    Ref<Window> window{ nullptr };
    Ref<Game> gameInstance{ nullptr };
};

}  // namespace CAL

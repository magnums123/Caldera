#pragma once

#include <memory>

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
    Application(const AppInfo& appInfo, std::unique_ptr<Game> game);
    ~Application();

    void run();

   private:
    bool isRunning{ false };
    bool isSuspended{ false };

    bool useWindow{ true };

    std::unique_ptr<Window> window{ nullptr };
    std::unique_ptr<Game> gameInstance{ nullptr };
};

}  // namespace CAL

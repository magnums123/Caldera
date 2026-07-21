#pragma once

#include <memory>

#include "Defines.hpp"
#include "Platform/Platform.hpp"
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
    Application(const AppInfo& appInfo);
    ~Application();

    void run();

   private:
    bool running{ false };

    std::unique_ptr<Platform> platform{ nullptr };
    std::unique_ptr<Window> window{ nullptr };
};

}  // namespace CAL

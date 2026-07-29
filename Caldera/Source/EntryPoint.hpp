#pragma once

#include "Core/Application.h"
#include "Core/Logger.hpp"
#include "Game/Game.hpp"

namespace CAL
{

extern std::unique_ptr<Game> createGame();
}
int main()
{
    CAL::AppInfo appInfo{ "Caldera Engine", 1280, 720, false };

    try
    {
        CAL::Application app{ appInfo, CAL::createGame() };
        app.run();
    }
    catch (const std::exception& e)
    {
        LOG_FATAL("{}", e.what());
    }

    return 0;
}

#if defined(_WIN32)
#define WIN32_MEAN_AND_LEAN
#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) { return main(); }

#endif

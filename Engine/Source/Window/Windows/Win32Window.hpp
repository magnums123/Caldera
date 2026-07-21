#pragma once

#if defined(_WIN32)
// clang-format off
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <minwindef.h>
// clang-format on

#include "Window/Window.hpp"

namespace CAL
{
class Win32Window : public Window
{
   public:
    Win32Window(const WindowCreateInfo& createInfo);
    ~Win32Window() override;

    void toggleFullscreen() override;
    void close() override;
    bool shouldClose() override;
    void update(float deltaTime) override;

   private:
    static bool classRegistered;
    static HINSTANCE hInstance;
    static const char* className;

    static void registerClass();
};

#endif
}  // namespace CAL

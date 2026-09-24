#pragma once

#include <Defines.hpp>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include "../Window.hpp"

namespace CAL
{

class X11Window : public Window
{
  public:
  X11Window(const WindowCreateInfo& createInfo);
  ~X11Window() = default;

  void toggleFullscreen() override;
  bool shouldClose() override;
  void close() override;
  void update() override;

  private:
  xcb_window_t windowID{0};
};

} // namespace CAL

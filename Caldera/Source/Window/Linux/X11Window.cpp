#include <Core/Asserts.hpp>
#include <Core/Logger.hpp>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include "X11Window.hpp"

namespace CAL
{

struct XCBState
{
  static xcb_connection_t* connection;
  static const xcb_setup_t* setup;
  static const xcb_screen_t* screen;

  XCBState()
  {
    int screenNum{0};

    connection = xcb_connect(nullptr, &screenNum);

    ASSERT_MSG(connection != nullptr, "Failed to create XCB connection");
    ASSERT_MSG(xcb_connection_has_error(connection) == 0, "Failed to connect to X11 server");

    setup = xcb_get_setup(connection);
    ASSERT_MSG(setup != nullptr, "Failed to get xcb_setup_t");

    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
    for (auto i{0}; i < screenNum; i++)
      xcb_screen_next(&iter);

    screen = iter.data;
    ASSERT_MSG(screen != nullptr, "Failed to get xcb_screen_t");
  }

  ~XCBState()
  {
    xcb_disconnect(connection);
  }
};

xcb_connection_t* XCBState::connection{nullptr};
const xcb_setup_t* XCBState::setup{nullptr};
const xcb_screen_t* XCBState::screen{nullptr};

Ref<Window> Window::Create(const WindowCreateInfo& createInfo)
{
  return CreateRef<X11Window>(Memory::MemoryTag::APPLICATION, createInfo);
}

X11Window::X11Window(const WindowCreateInfo& createInfo)
    : Window(createInfo)
{
  // debugBreak();
  windowID = xcb_generate_id(XCBState::connection);
  // clang-format off
  auto windowCookie = xcb_create_window(
      XCBState::connection, XCB_COPY_FROM_PARENT, 
      windowID, XCBState::screen->root, width/2, height/2,
      width, height, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
      XCBState::screen->root_visual, 0, nullptr
      );
  // clang-format on
  xcb_map_window(XCBState::connection, windowID);
  xcb_flush(XCBState::connection);

  handle = &windowID;
  internalState = XCBState::connection;
}

void X11Window::toggleFullscreen() {}

bool X11Window::shouldClose()
{
  return false;
}

void X11Window::close()
{
  xcb_destroy_window(XCBState::connection, windowID);
}

void X11Window::update() {}

} // namespace CAL

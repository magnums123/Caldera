#include <Core/Asserts.hpp>
#include <Core/Logger.hpp>

#include <unistd.h>

#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <xcb/xcb_keysyms.h>

#include "X11Window.hpp"

#include "Core/Event/Event.hpp"
#include "Core/Event/WindowEvents.hpp"
#include "Core/Event/KeyEvents.hpp"
#include "Core/Event/KeyCodes.hpp"

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

    ASSERT_MSG(xcb_connection_has_error(connection) == 0,
               "Failed to connect to X11 server");

    setup = xcb_get_setup(connection);

    ASSERT_MSG(setup != nullptr, "Failed to get xcb_setup_t");

    auto iter = xcb_setup_roots_iterator(setup);

    for (auto i{0}; i < screenNum; ++i)
      xcb_screen_next(&iter);

    screen = iter.data;

    ASSERT_MSG(screen != nullptr, "Failed to get xcb_screen_t");
  }

  ~XCBState()
  {
    if (connection)
    {
      xcb_disconnect(connection);
      connection = nullptr;
    }
  }
};

xcb_connection_t* XCBState::connection{nullptr};
const xcb_setup_t* XCBState::setup{nullptr};
const xcb_screen_t* XCBState::screen{nullptr};

static XCBState xcbState{};

Ref<Window> Window::Create(const WindowCreateInfo& createInfo)
{
  return CreateRef<X11Window>(Memory::MemoryTag::APPLICATION, createInfo);
}

X11Window::X11Window(const WindowCreateInfo& createInfo)
    : Window(createInfo)
{
  keySymbols = xcb_key_symbols_alloc(XCBState::connection);

  ASSERT_MSG(keySymbols != nullptr, "Failed to allocate XCB key symbols");

  windowID = xcb_generate_id(XCBState::connection);

  uint32_t eventMask = XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                       XCB_EVENT_MASK_STRUCTURE_NOTIFY;

  xcb_create_window(XCBState::connection, XCB_COPY_FROM_PARENT, windowID,
                    XCBState::screen->root, 0, 0, width, height, 0,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT, XCBState::screen->root_visual,
                    XCB_CW_EVENT_MASK, &eventMask);

  xcb_change_property(XCBState::connection, XCB_PROP_MODE_REPLACE, windowID,
                      XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, createInfo.name.size(),
                      createInfo.name.data());

  auto protocolsCookie = xcb_intern_atom(XCBState::connection, 0, 12, "WM_PROTOCOLS");

  auto deleteCookie = xcb_intern_atom(XCBState::connection, 0, 16, "WM_DELETE_WINDOW");

  auto* protocolsReply =
      xcb_intern_atom_reply(XCBState::connection, protocolsCookie, nullptr);

  auto* deleteReply = xcb_intern_atom_reply(XCBState::connection, deleteCookie, nullptr);

  ASSERT_MSG(protocolsReply != nullptr && deleteReply != nullptr,
             "Failed to get X11 window manager atoms");

  wmProtocols = protocolsReply->atom;
  wmDeleteWindow = deleteReply->atom;

  free(protocolsReply);
  free(deleteReply);

  xcb_change_property(XCBState::connection, XCB_PROP_MODE_REPLACE, windowID, wmProtocols,
                      XCB_ATOM_ATOM, 32, 1, &wmDeleteWindow);

  xcb_map_window(XCBState::connection, windowID);

  xcb_flush(XCBState::connection);

  handle = &windowID;
  internalState = XCBState::connection;
}

X11Window::~X11Window()
{
  if (windowID != 0)
  {
    xcb_destroy_window(XCBState::connection, windowID);

    xcb_flush(XCBState::connection);

    windowID = 0;
  }

  if (keySymbols)
  {
    xcb_key_symbols_free(keySymbols);
    keySymbols = nullptr;
  }
}

void X11Window::toggleFullscreen() {}

bool X11Window::shouldClose()
{
  return m_shouldClose;
}

void X11Window::close()
{
  m_shouldClose = true;
}

void X11Window::update()
{
  xcb_generic_event_t* genericEvent{nullptr};

  while ((genericEvent = xcb_poll_for_event(XCBState::connection)) != nullptr)
  {
    switch (genericEvent->response_type & ~0x80)
    {
      // --------------------------------------------------
      // Keyboard
      // --------------------------------------------------

    case XCB_KEY_PRESS:
    case XCB_KEY_RELEASE:
    {
      auto* event = reinterpret_cast<xcb_key_press_event_t*>(genericEvent);

      xcb_keysym_t keySym = xcb_key_symbols_get_keysym(keySymbols, event->detail, 0);

      KeyCodeID keyCode = KeyCodeDetail::KeyCodeFromLinuxKeySym(keySym);

      if ((genericEvent->response_type & ~0x80) == XCB_KEY_PRESS)
      {
        dispatcher.dispatch(KeyPressEvent{keyCode});
      }
      else
      {
        dispatcher.dispatch(KeyReleaseEvent{keyCode});
      }
    }
    break;

      // --------------------------------------------------
      // Window close
      // --------------------------------------------------

    case XCB_CLIENT_MESSAGE:
    {
      auto* event = reinterpret_cast<xcb_client_message_event_t*>(genericEvent);

      if (event->type == wmProtocols &&
          static_cast<xcb_atom_t>(event->data.data32[0]) == wmDeleteWindow)
      {
        close();

        dispatcher.dispatch(WindowCloseEvent{});
      }
    }
    break;

      // --------------------------------------------------
      // Window resize
      // --------------------------------------------------

    case XCB_CONFIGURE_NOTIFY:
    {
      auto* event = reinterpret_cast<xcb_configure_notify_event_t*>(genericEvent);

      if (event->width != width || event->height != height)
      {
        width = event->width;
        height = event->height;

        dispatcher.dispatch(WindowResizeEvent{width, height});
      }
    }
    break;
    }

    free(genericEvent);
  }
}

} // namespace CAL

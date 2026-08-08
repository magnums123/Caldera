#include "Win32Window.hpp"

#include <winuser.h>

#include <exception>

#include "Core/Asserts.hpp"
#include "Core/Event/KeyCodes.hpp"
#include "Core/Event/KeyEvents.hpp"
#include "Core/Event/MouseEvents.hpp"
#include "Core/Event/WindowEvents.hpp"
#include "Core/Logger.hpp"
#include "Core/Memory.hpp"

#if defined(_WIN32)

namespace CAL
{
bool Win32Window::classRegistered = false;
HINSTANCE Win32Window::hInstance = GetModuleHandle(0);
const char* Win32Window::className = "Win32 Window Class";

Ref<Window> Window::Create(const WindowCreateInfo& createInfo)
{
    return CreateRef<Win32Window>(Memory::MemoryTag::APPLICATION, createInfo);
}

Win32Window::Win32Window(const WindowCreateInfo& createInfo) : Window(createInfo)
{
    if (!classRegistered) registerClass();

    DWORD style = WS_OVERLAPPEDWINDOW;

    RECT windowRect = { 0, 0, static_cast<LONG>(createInfo.width), static_cast<LONG>(createInfo.height) };
    AdjustWindowRect(&windowRect, style, FALSE);

    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;

    handle = CreateWindowEx(
        0, className, createInfo.name.c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, nullptr,
        nullptr, hInstance, this);

    ASSERT_MSG(handle, "Failed to create Win32 Window.");
    internalState = hInstance;
    ShowWindow((HWND)handle, SW_SHOW);
};

void Win32Window::registerClass()
{
    HICON icon{ LoadIcon(hInstance, IDI_WINLOGO) };
    // Using nullptr in place of the hInstance in the LoadCursor function tells the OS that we want to manage the cursur
    // ourselfs Thus allowing to use the specified cursor type instead of the default Arrow
    HCURSOR cursor{ LoadCursor(nullptr, IDC_ARROW) };

    WNDCLASS wc{};
    wc.hIcon = icon;
    wc.hCursor = cursor;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = className;

    if (!RegisterClass(&wc))
    {
        LOG_FATAL("Failed to register Win32 Window");
        // Think of a better way to handle errors
        throw std::exception();
    }

    classRegistered = true;
}

Win32Window::~Win32Window()
{
    if (handle)
    {
        DestroyWindow(static_cast<HWND>(handle));
        handle = nullptr;
    }
}

void Win32Window::toggleFullscreen() {}

bool Win32Window::shouldClose() { return closeRequested; }

void Win32Window::close()
{
    closeRequested = true;
    PostQuitMessage(0);
}

void Win32Window::update()
{
    MSG msg{};
    while (PeekMessage(&msg, (HWND)handle, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

LRESULT CALLBACK Win32Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Win32Window* window{ nullptr };

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT* createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        window = reinterpret_cast<Win32Window*>(createStruct->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
    }
    else
        window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    switch (uMsg)
    {
        case WM_ERASEBKGND:
            return 1;  // 1 specifically tells the os that this will be handled by the app(read docs)
        case WM_CLOSE:
            window->dispatcher.dispatch(WindowCloseEvent{});
            if (window) window->close();
            return 0;
        case WM_DESTROY:
        {
            window->dispatcher.dispatch(WindowCloseEvent{});

            if (window) window->close();
            return 0;
        }
        case WM_SIZE:
        {
            RECT rect;
            GetClientRect(hwnd, &rect);
            uint32_t newWidth = rect.right - rect.left;
            uint32_t newHeight = rect.bottom - rect.top;

            if (newWidth > 0 || newHeight > 0) window->dispatcher.dispatch(WindowResizeEvent{ newWidth, newHeight });
        }
        break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            bool pressed = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);

            if ((uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN))
            {
                window->dispatcher.dispatch(KeyPressEvent{ KeyCodeFromWParam(wParam) });
            }
            else if ((uMsg == WM_KEYUP || uMsg == WM_SYSKEYUP))
            {
                window->dispatcher.dispatch(KeyReleaseEvent{ KeyCodeFromWParam(wParam) });
            }
        }
        break;
        case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            window->dispatcher.dispatch(MouseMoveEvent{ x, y });
        }
        break;
        case WM_MOUSEWHEEL:
        {
            int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (zDelta != 0)
            {
                zDelta = (zDelta < 0) ? -1 : 1;
                window->dispatcher.dispatch(MouseWheelEvent{ zDelta });
            }
        }
        break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP:
        {
            bool pressed = (uMsg == WM_LBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_RBUTTONDOWN);

            switch (uMsg)
            {
                case WM_LBUTTONDOWN:
                {
                    window->dispatcher.dispatch(MouseButtonPressEvent{ MouseButton::Left });
                }
                break;
                case WM_MBUTTONDOWN:
                {
                    window->dispatcher.dispatch(MouseButtonPressEvent{ MouseButton::Middle });
                }
                break;
                case WM_RBUTTONDOWN:
                {
                    window->dispatcher.dispatch(MouseButtonPressEvent{ MouseButton::Right });
                }
                break;
                case WM_LBUTTONUP:
                {
                    window->dispatcher.dispatch(MouseButtonReleaseEvent{ MouseButton::Left });
                }
                break;
                case WM_MBUTTONUP:
                {
                    window->dispatcher.dispatch(MouseButtonReleaseEvent{ MouseButton::Middle });
                }
                break;
                case WM_RBUTTONUP:
                {
                    window->dispatcher.dispatch(MouseButtonReleaseEvent{ MouseButton::Right });
                }
                break;
            }
        }
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

}  // namespace CAL

#endif

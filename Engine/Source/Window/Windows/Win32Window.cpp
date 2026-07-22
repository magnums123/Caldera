#include "Win32Window.hpp"

#include <exception>

#include "Core/Asserts.hpp"
#include "Core/Logger.hpp"

#if defined(_WIN32)

namespace CAL
{
bool Win32Window::classRegistered = false;
HINSTANCE Win32Window::hInstance = GetModuleHandle(0);
const char* Win32Window::className = "Win32 Window Class";

std::unique_ptr<Window> Window::Create(const WindowCreateInfo& createInfo)
{
    return std::make_unique<Win32Window>(createInfo);
}

Win32Window::Win32Window(const WindowCreateInfo& createInfo) : Window(createInfo)
{
    if (!classRegistered) registerClass();

    handle = CreateWindowEx(
        0, className, createInfo.name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, nullptr, nullptr, hInstance, this);

    ASSERT_MSG(handle, "Failed to create Win32 Window.");
    ShowWindow((HWND)handle, SW_SHOW);
    LOG_DEBUG("Succesfully created Win32Window.");
};

void Win32Window::registerClass()
{
    HICON icon{ LoadIcon(hInstance, IDI_APPLICATION) };
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
    LOG_DEBUG("Succesfully registered Window Class.");
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
    PostQuitMessage(0);
    closeRequested = true;
}

void Win32Window::update(float deltaTime)
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
            // 1 specifically tells the os that this will be handled by the app(read docs)
            return 1;
        // case WM_CLOSE:
        //     // TODO: Fire an event to tell Application a Window was closed
        //     if (window) window->close();
        //     return 0;
        case WM_DESTROY:
        {
            if (window) window->close();

            // PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE:
        {
            RECT rect;
            GetClientRect(hwnd, &rect);
            auto newWidth = rect.right - rect.left;
            auto newHeight = rect.bottom - rect.top;
            // TODO: Fire Window Resize event
        }
        break;
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            bool pressed = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
            // TODO: Fire keypress and keyrelease event
        }
        break;
        case WM_MOUSEMOVE:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            // TODO: Fire Mouse Move event
        }
        break;
        case WM_MOUSEWHEEL:
        {
            int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            if (zDelta != 0)
            {
                zDelta = (zDelta < 0) ? -1 : 1;
                // TODO: Fire Mouse Scroll event
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
            // TODO: Fire Mouse Button Event
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

}  // namespace CAL

#endif

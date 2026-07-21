#include <Core/Asserts.hpp>
#include <Core/Logger.hpp>
#include <Window/Window.hpp>

int main()
{
    // LOG_FATAL("A test message: {}", 3.1419f);
    // LOG_ERROR("A test message: {}", 3.1419f);
    // LOG_WARN("A test message: {}", 3.1419f);
    // LOG_INFO("A test message: {}", 3.1419f);
    // LOG_DEBUG("A test message: {}", 3.1419f);
    // LOG_TRACE("A test message: {}", 3.1419f);

    CAL::WindowCreateInfo windowCreateInfo{
        "Test Window",
        1280,
        720,
    };
    CAL::Window* window = CAL::Window::Create(windowCreateInfo);

    if (window) window->update(0);

    delete window;

    // ASSERT_MSG_DEBUG(false, "ASSERTION ENABLED");

    return 0;
}

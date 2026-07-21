#include <Core/Asserts.hpp>
#include <Core/Logger.hpp>

#include "Core/Application.h"

int main()
{
    // LOG_FATAL("A test message: {}", 3.1419f);
    // LOG_ERROR("A test message: {}", 3.1419f);
    // LOG_WARN("A test message: {}", 3.1419f);
    // LOG_INFO("A test message: {}", 3.1419f);
    // LOG_DEBUG("A test message: {}", 3.1419f);
    // LOG_TRACE("A test message: {}", 3.1419f);

    CAL::AppInfo appInfo{ "Test Window", 1280, 720, true };

    CAL::Application app{ appInfo };
    app.run();

    // ASSERT_MSG_DEBUG(false, "ASSERTION ENABLED");

    return 0;
}

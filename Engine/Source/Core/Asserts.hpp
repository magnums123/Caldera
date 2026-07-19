#pragma once

#include <source_location>

#include "Defines.hpp"
#include "Utility/String.hpp"

#define CALASSERTIONS_ENABLED
namespace CAL
{

void report_assertion_failure(StringView expression, StringView message, std::source_location loc);

#if defined(CALASSERTIONS_ENABLED)

#if defined(_WIN32) || defined(_WIN64)
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

#define ASSERT(expr)                                                                   \
    do                                                                                 \
    {                                                                                  \
        if (expr)                                                                      \
        {                                                                              \
        }                                                                              \
        else                                                                           \
        {                                                                              \
            CAL::report_assertion_failure(#expr, "", std::source_location::current()); \
            debugBreak();                                                              \
        }                                                                              \
    } while (false)

#define ASSERT_MSG(expr, message)                                                           \
    do                                                                                      \
    {                                                                                       \
        if (expr)                                                                           \
        {                                                                                   \
        }                                                                                   \
        else                                                                                \
        {                                                                                   \
            CAL::report_assertion_failure(#expr, message, std::source_location::current()); \
            debugBreak();                                                                   \
        }                                                                                   \
    } while (false)

#if defined(DEBUG)
#define ASSERT_DEBUG(expr)                                                             \
    do                                                                                 \
    {                                                                                  \
        if (expr)                                                                      \
        {                                                                              \
        }                                                                              \
        else                                                                           \
        {                                                                              \
            CAL::report_assertion_failure(#expr, "", std::source_location::current()); \
            debugBreak();                                                              \
        }                                                                              \
    } while (false)

#define ASSERT_MSG_DEBUG(expr, message)                                                     \
    do                                                                                      \
    {                                                                                       \
        if (expr)                                                                           \
        {                                                                                   \
        }                                                                                   \
        else                                                                                \
        {                                                                                   \
            CAL::report_assertion_failure(#expr, message, std::source_location::current()); \
            debugBreak();                                                                   \
        }                                                                                   \
    } while (false)
#else
#define ASSERT_DEBUG(expr)
#define ASSERT_MSG_DEBUG(expr, message)
#endif

#else
#define ASSERT(expr)
#define ASSERT_MSG(expr, message)
#define ASSERT_DEBUG(expr)
#define ASSERT_MSG_DEBUG(expr, message)
#endif

}  // namespace CAL

#pragma once

#include <format>
#include <source_location>
#include <utility>

#include "Defines.hpp"
#include "Utility/String.hpp"

namespace CAL
{
inline constexpr bool LOG_FATAL_ENABLED = true;
inline constexpr bool LOG_ERROR_ENABLED = true;
inline constexpr bool LOG_WARN_ENABLED = true;
inline constexpr bool LOG_INFO_ENABLED = true;

#if DEBUG == 0
inline constexpr bool LOG_DEBUG_ENABLED = true;
inline constexpr bool LOG_TRACE_ENABLED = true;
#else
inline constexpr bool LOG_DEBUG_ENABLED = false;
inline constexpr bool LOG_TRACE_ENABLED = false;
#endif  // DEBUG

enum class LogLevel
{
    Fatal,
    Error,
    Warn,
    Info,
    Debug,
    Trace,
};

// Formats `fmt` with `args` via std::format and dispatches it at
// `level`, tagged with `loc`. `fmt` is a std::format_string<Args...>,
// so a malformed format string (wrong placeholder count, bad type) is a
// *compile* error rather than a runtime crash -- something the old
// `...` C-varargs signature could never give you, since va_arg only
// has defined behaviour for a narrow set of POD-like types.
template <typename... Args>
void log(LogLevel level, std::source_location loc, std::format_string<Args...> fmt, Args&&... args)
{
    logOutput(level, loc, std::format(fmt, std::forward<Args>(args)...));
}

// Does the actual formatting/writing of an already-composed message.
void logOutput(LogLevel level, std::source_location loc, StringView message);

}  // namespace CAL

#define CAL_LOG_IMPL(level, enabled, fmt, ...)                                    \
    do                                                                            \
    {                                                                             \
        if constexpr (enabled)                                                    \
        {                                                                         \
            CAL::log(level, std::source_location::current(), fmt, ##__VA_ARGS__); \
        }                                                                         \
    } while (false)

#define LOG_FATAL(fmt, ...) CAL_LOG_IMPL(CAL::LogLevel::Fatal, CAL::LOG_FATAL_ENABLED, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) CAL_LOG_IMPL(CAL::LogLevel::Error, CAL::LOG_ERROR_ENABLED, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) CAL_LOG_IMPL(CAL::LogLevel::Warn, CAL::LOG_WARN_ENABLED, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) CAL_LOG_IMPL(CAL::LogLevel::Info, CAL::LOG_INFO_ENABLED, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) CAL_LOG_IMPL(CAL::LogLevel::Debug, CAL::LOG_DEBUG_ENABLED, fmt, ##__VA_ARGS__)
#define LOG_TRACE(fmt, ...) CAL_LOG_IMPL(CAL::LogLevel::Trace, CAL::LOG_TRACE_ENABLED, fmt, ##__VA_ARGS__)

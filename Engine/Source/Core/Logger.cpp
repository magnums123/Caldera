#include "Logger.hpp"

#include "Asserts.hpp"
#include "Utility/String.hpp"

namespace CAL
{

namespace
{

constexpr StringView LevelLabel(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Fatal:
            return "FATAL";
        case LogLevel::Error:
            return "ERROR";
        case LogLevel::Warn:
            return "WARN ";
        case LogLevel::Info:
            return "INFO ";
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Trace:
            return "TRACE";
    }
    return "?????";
}

// ANSI colour codes, one per level. If a target platform's console doesn't
// support them (older Windows terminals), swap this out for a no-op table.
constexpr StringView LevelColor(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Fatal:
            return "\x1b[41;97m";  // white on red
        case LogLevel::Error:
            return "\x1b[31m";  // red
        case LogLevel::Warn:
            return "\x1b[33m";  // yellow
        case LogLevel::Info:
            return "\x1b[32m";  // green
        case LogLevel::Debug:
            return "\x1b[36m";  // cyan
        case LogLevel::Trace:
            return "\x1b[90m";  // grey
    }
    return "\x1b[0m";
}

constexpr StringView ColorReset = "\x1b[0m";

FILE* StreamFor(LogLevel level) { return (level == LogLevel::Fatal || level == LogLevel::Error) ? stderr : stdout; }

}  // namespace

// bool Logger::initialize()
// {
//     // Hook a log file / other sinks here if needed later.
//     return true;
// }
//
// void Logger::shutdown()
// {
//     // Flush/close sinks here if any get added.
// }
//
void logOutput(LogLevel level, std::source_location loc, StringView message)
{
    FILE* stream = StreamFor(level);

    String formatted = std::format(
        "{}[{}] {}:{} ({}) - {}{}\n", LevelColor(level), LevelLabel(level), loc.file_name(), loc.line(),
        loc.function_name(), message, ColorReset);

    std::fputs(formatted.c_str(), stream);

    if (level == LogLevel::Fatal)
    {
        std::fflush(stream);
        // Deliberately not calling std::abort()/std::terminate() here --
        // that's a policy decision for the caller (or a dedicated
        // LOG_FATAL-triggers-abort build option), not something the
        // logger should decide unilaterally.
    }
}

void report_assertion_failure(StringView expression, StringView message, std::source_location loc)
{
    log(LogLevel::Fatal, loc, "{}: {}", expression, message);
}

}  // namespace CAL

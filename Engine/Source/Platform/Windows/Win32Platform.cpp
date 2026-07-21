#include "Win32Platform.hpp"

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace CAL
{

static float clockFrequency;
static std::int64_t startTime;

std::unique_ptr<Platform> Platform::Create() { return std::make_unique<Win32Platform>(Win32Platform()); }

Win32Platform::Win32Platform()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clockFrequency = 1.f / static_cast<float>(frequency.QuadPart);
    QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
}

Win32Platform::~Win32Platform() {}

void* Win32Platform::allocate(size_t size, bool aligned) { return malloc(size); }
void Win32Platform::freeMemory(void* block, bool aligned) { free(block); }
void* Win32Platform::zeroMemory(void* block, size_t size) { return memset(block, 0, size); }
void Win32Platform::copyMemory(void* dst, void* src, size_t size) { memcpy(dst, src, size); }
void* Win32Platform::setMemory(void* dst, std::int32_t value, size_t size) { return memset(dst, value, size); }

float Win32Platform::getAbsoluteTime()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return (float)currentTime.QuadPart * clockFrequency;
}
void Win32Platform::sleep(uint64_t ms) { Sleep(ms); }

}  // namespace CAL
#endif

#include "X11Platform.hpp"

#include "Core/Memory.hpp"

#if defined(_WIN32)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace CAL
{

static float clockFrequency;
static std::int64_t startTime;

Ref<Platform> Platform::Create()
{
  return CreateRef<Win32Platform>(Memory::MemoryTag::APPLICATION);
}

Win32Platform::Win32Platform()
{
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  clockFrequency = 1.f / static_cast<float>(frequency.QuadPart);
  QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
}

Win32Platform::~Win32Platform() {}

StringView Win32Platform::getRequiredExtensions()
{
  return vk::KHRWin32SurfaceExtensionName;
}

float Win32Platform::getAbsoluteTime() const
{
  LARGE_INTEGER currentTime;
  QueryPerformanceCounter(&currentTime);
  return (float)currentTime.QuadPart * clockFrequency;
}
void Win32Platform::sleep(uint64_t ms) const
{
  Sleep(ms);
}

} // namespace CAL
#endif

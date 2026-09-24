#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <xcb/xcb.h>

#include "X11Platform.hpp"

#include "Core/Memory.hpp"

namespace CAL
{

static float clockFrequency;
static std::int64_t startTime;

Ref<Platform> Platform::Create()
{
  return CreateRef<X11Platform>(Memory::MemoryTag::APPLICATION);
}

X11Platform::X11Platform()
{
  // Start timer
}

X11Platform::~X11Platform() {}

StringView X11Platform::getRequiredExtensions()
{
  return "";
}

float X11Platform::getAbsoluteTime() const
{
  return 0.f;
}
void X11Platform::sleep(uint64_t ms) const
{
  sleep(ms);
}

} // namespace CAL

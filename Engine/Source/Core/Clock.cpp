#include "Clock.h"

namespace CAL
{

Clock::Clock(Ref<Platform>& platform) : platform(platform)
{
    startTime = platform->getAbsoluteTime();
    elapsedTime = 0.f;
}
Clock::~Clock() {}

void Clock::update()
{
    if (startTime != 0) elapsedTime = platform->getAbsoluteTime() - startTime;
}

void Clock::stop() { startTime = 0; }

}  // namespace CAL

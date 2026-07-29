#pragma once

#include <Defines.hpp>

#include "Core/Memory.hpp"
#include "Platform/Platform.hpp"

namespace CAL
{

class Clock
{
   public:
    Clock(Ref<Platform>& platform);
    ~Clock();

    inline float getStartTime() const { return startTime; }
    inline float getElapsedTime() const { return elapsedTime; }

    void update();
    void stop();

   private:
    Ref<Platform>& platform;
    float startTime{};
    float elapsedTime{};
};

}  // namespace CAL

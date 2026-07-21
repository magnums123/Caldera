#pragma once

#include <cstdint>

#include "Defines.hpp"

namespace CAL
{

class Platform
{
   public:
    Platform() {}
    virtual ~Platform() = default;

    virtual void* allocate(size_t size, bool aligned) = 0;
    virtual void freeMemory(void* block, bool aligned) = 0;
    virtual void* zeroMemory(void* block, size_t size) = 0;
    virtual void copyMemory(void* dst, void* src, size_t size) = 0;
    virtual void* setMemory(void* dst, std::int32_t value, size_t size) = 0;

    virtual float getAbsoluteTime() = 0;
    virtual void sleep(uint64_t ms) = 0;

    static Platform* Create();
};
}  // namespace CAL

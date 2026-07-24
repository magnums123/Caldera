#pragma once

#include <cstdint>
#include <memory>

#include "Core/Memory.hpp"
#include "Defines.hpp"

namespace CAL
{

class Platform
{
   public:
    Platform() {}
    virtual ~Platform() = default;

    virtual void* allocateMemory(size_t size, bool aligned) const = 0;
    virtual void freeMemory(void* block, bool aligned) const = 0;
    virtual void* zeroMemory(void* block, size_t size) const = 0;
    virtual void copyMemory(void* dst, void* src, size_t size) const = 0;
    virtual void* setMemory(void* dst, std::int32_t value, size_t size) const = 0;

    virtual float getAbsoluteTime() const = 0;
    virtual void sleep(uint64_t ms) const = 0;

    static Ref<Platform> Create();
};
}  // namespace CAL

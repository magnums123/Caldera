
#pragma once

#if defined(_WIN32)
// clang-format off
// clang-format on

#include "Platform/Platform.hpp"

namespace CAL
{
class Win32Platform : public Platform
{
   public:
    Win32Platform();
    ~Win32Platform() override;
    void* allocate(size_t size, bool aligned) override;
    void freeMemory(void* block, bool aligned) override;
    void* zeroMemory(void* block, size_t size) override;
    void copyMemory(void* dst, void* src, size_t size) override;
    void* setMemory(void* dst, std::int32_t value, size_t size) override;

    float getAbsoluteTime() override;
    void sleep(uint64_t ms) override;
};

#endif
}  // namespace CAL

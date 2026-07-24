#include "Memory.hpp"

#include <sys/stat.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <format>
#include <memory>

#include "Core/Logger.hpp"
#include "Platform/Platform.hpp"

namespace CAL::Memory
{
struct MemoryStats
{
    size_t totalAllocated{ 0 };
    std::array<size_t, (size_t)MemoryTag::MAX_TAGS> taggedALlocations{};
};

static Ref<Platform> platform{ nullptr };
static MemoryStats stats{};

// clang-format off
static std::array<String, (size_t)MemoryTag::MAX_TAGS+1> MemoryTagString
{
   "UNKNOWN          ",
   "ARRAY            ",
   "DARRAY           ",
   "DICTIONARY       ",
   "QUEUE            ",
   // "BST",
   "STRING           ",
   "APPLICATION      ",
   "JOB              ",
   "TEXTURE          ",
   "MATERIAL_INSTANCE",
   "RENDERER         ",
   "GAME             ",
   "TRANSFORM        ",
   "ENTITY           ",
   "ENTITY_NODE      ",
   "SCENE            ",

   "MAX_MemoryTagString[i]S         "
};
// clang-format on

void initMemory() { platform = Platform::Create(); }

void shutdownMemory() {}

void* allocateMemory(size_t size, MemoryTag tag)
{
    if (tag == MemoryTag::UNKNOWN) LOG_WARN("allocateMemory called using MemoryTag::UNKNOWN");

    stats.totalAllocated += size;
    stats.taggedALlocations[(size_t)tag] += size;

    // TODO: Memory alignment
    // auto block = platform->allocateMemory(size, false);
    // return platform->zeroMemory(block, size);
    return platform->allocateMemory(size, false);
}

void freeMemory(void* block, size_t size, MemoryTag tag)
{
    if (tag == MemoryTag::UNKNOWN) LOG_WARN("freeMemory called using MemoryTag::UNKNOWN");

    stats.totalAllocated -= size;
    stats.taggedALlocations[(size_t)tag] -= size;

    // TODO: Memory alignment
    platform->freeMemory(block, false);
}

void* zeroMemory(void* block, size_t size) { return platform->zeroMemory(block, size); }

void copyMemory(void* dst, void* src, size_t size) { platform->copyMemory(dst, src, size); }

void* setMemory(void* dst, std::int32_t value, size_t size) { return platform->setMemory(dst, value, size); }

String getMemoryUsageString()
{
    const uint64_t gib = 1024 * 1024 * 1024;
    const uint64_t mib = 1024 * 1024;
    const uint64_t kib = 1024;
    String usageString{ "System memory use:\n" };

    for (size_t i = 0; i < (size_t)MemoryTag::MAX_TAGS; i++)
    {
        String unit{ "xiB" };
        float amount{ 1.f };

        if (stats.taggedALlocations[i] >= gib)
        {
            unit[0] = 'G';
            amount = stats.taggedALlocations[i] / (float)gib;
        }
        else if (stats.taggedALlocations[i] >= mib)
        {
            unit[0] = 'M';
            amount = stats.taggedALlocations[i] / (float)mib;
        }
        else if (stats.taggedALlocations[i] >= kib)
        {
            unit[0] = 'K';
            amount = stats.taggedALlocations[i] / (float)kib;
        }
        else
        {
            unit = 'B';
            amount = stats.taggedALlocations[i];
        }

        usageString.append(std::format("{}: {}{}\n", MemoryTagString[i], amount, unit));
    }
    return usageString + "\n";
}

}  // namespace CAL::Memory

#include <format>
#include <memory>

#include "Memory.hpp"

#include "Core/Logger.hpp"

namespace CAL::Memory
{
struct MemoryStats
{
  size_t totalAllocated{0};
  std::array<size_t, (size_t)MemoryTag::MAX_TAGS> taggedALlocations{};
};

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
};
// clang-format on

void initMemory() {}

void shutdownMemory() {}

void* allocateMemory(size_t size, MemoryTag tag)
{
  if (tag == MemoryTag::UNKNOWN)
    LOG_WARN("allocateMemory called using MemoryTag::UNKNOWN");

  stats.totalAllocated += size;
  stats.taggedALlocations[(size_t)tag] += size;

  // TODO: Memory alignment
  // auto block = platform->allocateMemory(size, false);
  // return platform->zeroMemory(block, size);
  return std::malloc(size);
}

void freeMemory(void* block, MemoryTag tag)
{
  if (tag == MemoryTag::UNKNOWN)
    LOG_WARN("freeMemory called using MemoryTag::UNKNOWN");

  // TODO: Memory alignment
  std::free(block);
}

void* zeroMemory(void* block, size_t size)
{
  return setMemory(block, 0, size);
}

void copyMemory(void* dst, void* src, size_t size)
{
  std::memcpy(dst, src, size);
}

void* setMemory(void* dst, std::int32_t value, size_t size)
{
  return std::memset(dst, value, size);
}

std::pair<String, float> getUnitAndAmount(float amountInBytes)
{
  const uint64_t gib = 1024 * 1024 * 1024;
  const uint64_t mib = 1024 * 1024;
  const uint64_t kib = 1024;

  String unit{"xiB"};
  float amount{1.f};

  if (amountInBytes >= gib)
  {
    unit[0] = 'G';
    amount = amountInBytes / (float)gib;
  }
  else if (amountInBytes >= mib)
  {
    unit[0] = 'M';
    amount = amountInBytes / (float)mib;
  }
  else if (amountInBytes >= kib)
  {
    unit[0] = 'K';
    amount = amountInBytes / (float)kib;
  }
  else
  {
    unit = 'B';
    amount = amountInBytes;
  }

  return {unit, amount};
}

void reduceTaggedandTotalAllocation(Memory::MemoryTag tag, size_t size)
{
  if (tag == MemoryTag::UNKNOWN)
    LOG_WARN("freeMemory called using MemoryTag::UNKNOWN");

  stats.totalAllocated -= size;
  stats.taggedALlocations[(size_t)tag] -= size;
}

String getMemoryUsageString()
{
  String usageString{"\nSystem memory use:\n"};

  for (size_t i = 0; i < (size_t)MemoryTag::MAX_TAGS; i++)
  {
    auto unitAndAmount{getUnitAndAmount(stats.taggedALlocations[i])};

    usageString.append(
        std::format("\t{}: {}{}\n", MemoryTagString[i], unitAndAmount.second, unitAndAmount.first));
  }
  auto unitAndAmount{getUnitAndAmount(stats.totalAllocated)};
  usageString.append(
      std::format("\tTOTAL Allocated  : {}{}", unitAndAmount.second, unitAndAmount.first));

  return usageString + "\n";
}

} // namespace CAL::Memory

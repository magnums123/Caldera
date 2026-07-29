#pragma once

#include <Defines.hpp>
#include <cstdint>
#include <memory>

#include "Utility/String.hpp"

namespace CAL
{

template <class Ty>
using Ref = std::shared_ptr<Ty>;

template <typename Ty, typename... Args>
constexpr Ref<Ty> CreateRef(Args&&... args)
{
    return std::make_shared<Ty>(std::forward<Args>(args)...);
}
// class Ref
// {
//    public:
//     Ref() : ptr(std::make_unique<Ty>()) {}
//
//    private:
//     std::unique_ptr<Ty> ptr{ nullptr };
// };
//
// template <class Ty>
// using sharedPtr = std::shared_ptr<Ty>;
namespace Memory
{

enum class MemoryTag
{
    UNKNOWN,
    ARRAY,
    DARRAY,
    DICTIONARY,
    QUEUE,
    // BST,
    STRING,
    APPLICATION,
    JOB,
    TEXTURE,
    MATERIAL_INSTANCE,
    RENDERER,
    GAME,
    TRANSFORM,
    ENTITY,
    ENTITY_NODE,
    SCENE,

    MAX_TAGS
};

void initMemory();
void shutdownMemory();

void* allocateMemory(size_t size, MemoryTag tag);
void freeMemory(void* block, size_t size, MemoryTag tag);
void* zeroMemory(void* block, size_t size);
void copyMemory(void* dst, void* src, size_t size);
void* setMemory(void* dst, std::int32_t value, size_t size);

String getMemoryUsageString();
}  // namespace Memory
}  // namespace CAL

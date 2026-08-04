#pragma once

#include <Defines.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "Utility/String.hpp"

namespace CAL
{

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

template <class Ty>
using Ref = std::shared_ptr<Ty>;

template <class Ty>
class Allocator
{
   private:
    Memory::MemoryTag tag{ Memory::MemoryTag::UNKNOWN };

   public:
    typedef Ty value_type;

    Allocator(Memory::MemoryTag tag) : tag(tag) {}

    Memory::MemoryTag getTag() const noexcept { return tag; }

    template <class U>
    constexpr Allocator(const Allocator<U>& other) noexcept : tag(other.getTag())
    {
    }

    Ty* allocate(std::size_t n) noexcept { return (Ty*)Memory::allocateMemory(n * sizeof(Ty), tag); }

    void deallocate(Ty* p, std::size_t n) noexcept { Memory::freeMemory(p, n * sizeof(Ty), tag); }
};

template <typename Ty, typename... Args>
constexpr Ref<Ty> CreateRef(Memory::MemoryTag tag, Args&&... args)
{
    return std::allocate_shared<Ty>(Allocator<Ty>(tag), std::forward<Args>(args)...);
}

}  // namespace CAL

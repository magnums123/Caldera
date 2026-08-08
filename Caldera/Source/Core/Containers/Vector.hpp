// FIX: File was written by AI
#pragma once

#include <Defines.hpp>
#include <initializer_list>
#include <utility>

#include "Core/Asserts.hpp"
#include "Core/Memory.hpp"

namespace CAL
{

using namespace Memory;

constexpr size_t defaultCapacity{ 1 };

template <typename Ty>
class Vector
{
   public:
    Vector() : mCapacity(defaultCapacity), mData((Ty*)allocateMemory(defaultCapacity * sizeof(Ty), MemoryTag::DARRAY))
    {
    }

    Vector(size_t size) : mSize(size), mCapacity(size), mData((Ty*)allocateMemory(size * sizeof(Ty), MemoryTag::DARRAY))
    {
        for (size_t i = 0; i < mSize; ++i) constructAt(mData + i);
    }

    Vector(std::initializer_list<Ty> data)
        : mSize(data.size()),
          mCapacity(data.size()),
          mData((Ty*)allocateMemory(data.size() * sizeof(Ty), MemoryTag::DARRAY))
    {
        size_t i = 0;
        for (const Ty& item : data) constructAt(mData + i++, item);
    }

    Vector(const Vector& other)
        : mSize(other.mSize),
          mCapacity(other.mSize),
          mData((Ty*)allocateMemory(other.mSize * sizeof(Ty), MemoryTag::DARRAY))
    {
        for (size_t i = 0; i < mSize; ++i) constructAt(mData + i, other.mData[i]);
    }

    Vector(Vector&& other) noexcept : mSize(other.mSize), mCapacity(other.mCapacity), mData(other.mData)
    {
        other.mSize = 0;
        other.mCapacity = 0;
        other.mData = nullptr;
    }

    ~Vector() { releaseBuffer(); }

    Vector& operator=(const Vector& other)
    {
        if (this != &other)
        {
            Vector copy(other);
            swap(copy);
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept
    {
        if (this != &other)
        {
            releaseBuffer();
            mSize = other.mSize;
            mCapacity = other.mCapacity;
            mData = other.mData;
            other.mSize = 0;
            other.mCapacity = 0;
            other.mData = nullptr;
        }
        return *this;
    }

    void swap(Vector& other) noexcept
    {
        std::swap(mSize, other.mSize);
        std::swap(mCapacity, other.mCapacity);
        std::swap(mData, other.mData);
    }

    inline size_t size() const { return mSize; }
    inline size_t capacity() const { return mCapacity; }
    inline bool empty() const { return mSize == 0; }

    inline Ty* data() { return mData; }
    inline const Ty* data() const { return mData; }

    void resize(size_t size)
    {
        if (size > mCapacity) reallocateTo(size);
        if (size > mSize)
            for (size_t i = mSize; i < size; ++i) constructAt(mData + i);
        else
            for (size_t i = size; i < mSize; ++i) destroyAt(mData + i);
        mSize = size;
    }

    void reserve(size_t size)
    {
        if (size > mCapacity) reallocateTo(size);
    }

    void shrinkToFit()
    {
        if (mSize == mCapacity) return;
        if (mSize == 0)
        {
            releaseBuffer();
            return;
        }
        reallocateTo(mSize);
    }

    void pushBack(Ty data)
    {
        if (mSize == mCapacity) reallocateTo(mCapacity == 0 ? defaultCapacity : mCapacity * 2);
        constructAt(mData + mSize, std::move(data));
        mSize += 1;
    }

    void pop()
    {
        ASSERT_MSG(mSize > 0, "Cannot pop an empty Vector");
        if (mSize == 0) return;
        mSize -= 1;
        destroyAt(mData + mSize);
    }

    void clear()
    {
        destroyAll();
        mSize = 0;
    }

    void remove(size_t index)
    {
        ASSERT_MSG(index < mSize, "Invalid Index into Vector");
        if (index >= mSize) return;
        for (size_t i = index; i + 1 < mSize; ++i) mData[i] = std::move(mData[i + 1]);
        mSize -= 1;
        destroyAt(mData + mSize);
    }

    Ty& operator[](size_t index)
    {
        ASSERT_MSG(index < size(), "Invalid Index into Vector");
        return mData[index];
    }

    const Ty& operator[](size_t index) const
    {
        ASSERT_MSG(index < size(), "Invalid Index into Vector");
        return mData[index];
    }

    Ty* begin() { return mData; }
    const Ty* begin() const { return mData; }
    Ty* end() { return mData + mSize; }
    const Ty* end() const { return mData + mSize; }

   private:
    static void constructAt(Ty* ptr) { ::new (static_cast<void*>(ptr)) Ty(); }
    static void constructAt(Ty* ptr, const Ty& value) { ::new (static_cast<void*>(ptr)) Ty(value); }
    static void constructAt(Ty* ptr, Ty&& value) { ::new (static_cast<void*>(ptr)) Ty(std::move(value)); }
    static void destroyAt(Ty* ptr) { ptr->~Ty(); }

    void destroyAll()
    {
        for (size_t i = 0; i < mSize; ++i) destroyAt(mData + i);
    }

    // Destroys all live elements, frees the backing buffer with its exact allocated size, and resets to an empty state.
    void releaseBuffer()
    {
        destroyAll();
        if (mData) freeMemory(mData, mCapacity * sizeof(Ty), MemoryTag::DARRAY);
        mSize = 0;
        mCapacity = 0;
        mData = nullptr;
    }

    // Reallocates the backing buffer to exactly `newCapacity`, moving existing elements over.
    void reallocateTo(size_t newCapacity)
    {
        Ty* newData = (Ty*)allocateMemory(newCapacity * sizeof(Ty), MemoryTag::DARRAY);
        for (size_t i = 0; i < mSize; ++i) constructAt(newData + i, std::move(mData[i]));
        destroyAll();
        if (mData) freeMemory(mData, mCapacity * sizeof(Ty), MemoryTag::DARRAY);
        mData = newData;
        mCapacity = newCapacity;
    }

    size_t mSize{};
    size_t mCapacity{};
    Ty* mData{};
};

}  // namespace CAL

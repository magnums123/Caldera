#pragma once

#include <Defines.hpp>
#include <algorithm>
#include <initializer_list>

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
    Vector() : mCapacity(defaultCapacity), mData((Ty*)allocateMemory(1 * sizeof(Ty), MemoryTag::DARRAY)) {}
    Vector(size_t size) : mSize(size), mCapacity(size), mData((Ty*)allocateMemory(size * sizeof(Ty), MemoryTag::DARRAY))
    {
        ASSERT_MSG(size > -1, "Invalid Vector Size");
    }
    Vector(std::initializer_list<Ty> data)
        : mSize(data.size()),
          mCapacity(data.size()),
          mData((Ty*)allocateMemory(data.size() * sizeof(Ty), MemoryTag::DARRAY))
    {
        std::copy(data.begin(), data.end(), mData);
    }

    ~Vector()
    {
        if (mData) freeMemory(mData, mSize * sizeof(Ty), MemoryTag::DARRAY);
    }

    inline size_t size() const { return mSize; }
    inline Ty* data() const { return mData; }

    void resize(size_t size)
    {
        auto temp = allocateMemory(size * sizeof(Ty), MemoryTag::DARRAY);
        copyMemory(temp, mData, size * sizeof(Ty));
        freeMemory(mData, (mSize - 1) * sizeof(Ty), MemoryTag::DARRAY);
        mData = (Ty*)temp;
        mSize = size;
        if (mSize > mCapacity) mCapacity = mSize;
    }

    // TODO: Implement these functions
    void reserve(size_t size) {}
    void shrinkToFit() {}

    void pushBack(Ty data)
    {
        auto temp = allocateMemory((mSize + 1) * sizeof(Ty), MemoryTag::DARRAY);
        copyMemory(temp, mData, mSize * sizeof(Ty));
        freeMemory(mData, mSize * sizeof(Ty), MemoryTag::DARRAY);
        mSize += 1;
        mData = (Ty*)temp;
        // resize(mSize + 1);
        // copyMemory(&mData[mSize - 1], &data, sizeof(Ty));
        mData[mSize - 1] = data;
        if (mSize > mCapacity) mCapacity = mSize;
    }
    void pop() { resize(mSize - 1); }

    void remove(uint64_t index)
    {
        if (index == mSize - 1)
        {
            pop();
            return;
        }
        if (index == 0)
        {
            auto newMemAlloc = allocateMemory((mSize - 1) * sizeof(Ty), MemoryTag::DARRAY);
            copyMemory(newMemAlloc, mData + 1, (mSize - 1) * sizeof(Ty));

            freeMemory(mData, mSize * sizeof(Ty), MemoryTag::DARRAY);
            mData = (Ty*)newMemAlloc;
            mSize -= 1;
            return;
        }
        size_t size{ (mSize - index) * sizeof(Ty) };

        auto start = allocateMemory(index * sizeof(Ty), MemoryTag::DARRAY);
        auto end = allocateMemory(size, MemoryTag::DARRAY);

        copyMemory(start, mData, index * sizeof(Ty));
        copyMemory(end, &mData[index], size);

        freeMemory(mData, mSize * sizeof(Ty), MemoryTag::DARRAY);

        mData = (Ty*)allocateMemory(mSize - 1, MemoryTag::DARRAY);

        copyMemory(mData, start, index * sizeof(Ty));
        copyMemory(mData + index, end, size);

        freeMemory(start, index * sizeof(Ty), MemoryTag::DARRAY);
        freeMemory(end, size, MemoryTag::DARRAY);

        mSize -= 1;
    }

    Ty& operator[](uint64_t index) const
    {
        ASSERT_MSG((index > -1) && (index < size()), "Invalid Index into Vector");
        return mData[index];
    }

    Ty* begin() { return mData; }
    Ty* end() { return &mData[mSize]; }

   private:
    size_t mSize{};
    size_t mCapacity{};
    Ty* mData{};
};

}  // namespace CAL

#pragma once

#include <Defines.hpp>

#include "Core/Memory.hpp"

namespace CAL
{

enum class BufferUsage
{
    Vertex,
    Index,
    Uniform,
    Storage
};

enum class RendererBackendType;

struct BufferInfo
{
    BufferUsage usage;
    RendererBackendType backendType;
    size_t size;
    void* backendData;
};

class Buffer
{
   public:
    Buffer() = default;
    virtual void setData(void* data, size_t size) = 0;

    static Ref<Buffer> Create(const BufferInfo& bufferInfo);

   protected:
    BufferUsage usage;
};

}  // namespace CAL

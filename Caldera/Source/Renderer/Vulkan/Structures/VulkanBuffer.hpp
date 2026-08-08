#pragma once

#include <Defines.hpp>
#include <vk_mem_alloc.hpp>

#include "Renderer/Structures/Buffer.hpp"
#include "vulkan/vulkan.hpp"

namespace CAL
{

struct VulkanBuffer : public Buffer
{
    VulkanBuffer(const BufferInfo& bufferInfo);
    void setData(void* data, size_t size) override;

   private:
    vk::Buffer buffer{ nullptr };
    vma::Allocation bufferAllocation{ nullptr };
    vma::AllocationInfo bufferAllocationInfo{};
};

}  // namespace CAL

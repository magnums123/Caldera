#include "VulkanBuffer.hpp"

#include <vk_mem_alloc_enums.hpp>
#include <vk_mem_alloc_structs.hpp>

#include "Core/Logger.hpp"
#include "Core/Memory.hpp"
#include "Renderer/Structures/Buffer.hpp"
#include "Renderer/Vulkan/VulkanTypes.hpp"
#include "vulkan/vulkan.hpp"

namespace CAL
{

VulkanBuffer::VulkanBuffer(const BufferInfo& bufferInfo) : Buffer()
{
    auto context = reinterpret_cast<VulkanContext*>(bufferInfo.backendData);
    usage = bufferInfo.usage;

    vk::BufferUsageFlags usageFlags;
    vma::AllocationCreateFlags allocationFlags;

    switch (bufferInfo.usage)
    {
        case BufferUsage::Vertex:
            usageFlags |= vk::BufferUsageFlagBits::eVertexBuffer;
            allocationFlags |= vma::AllocationCreateFlagBits::eHostAccessSequentialWrite |
                               vma::AllocationCreateFlagBits::eHostAccessAllowTransferInstead |
                               vma::AllocationCreateFlagBits::eMapped;
            break;
        case BufferUsage::Index:
            usageFlags |= vk::BufferUsageFlagBits::eIndexBuffer;
            allocationFlags |= vma::AllocationCreateFlagBits::eHostAccessSequentialWrite |
                               vma::AllocationCreateFlagBits::eHostAccessAllowTransferInstead |
                               vma::AllocationCreateFlagBits::eMapped;
            break;
        case BufferUsage::Uniform:
            usageFlags |= vk::BufferUsageFlagBits::eUniformBuffer;
            allocationFlags |= vma::AllocationCreateFlagBits::eHostAccessSequentialWrite |
                               vma::AllocationCreateFlagBits::eHostAccessAllowTransferInstead |
                               vma::AllocationCreateFlagBits::eMapped;
            break;
        case BufferUsage::Storage:
            LOG_ERROR("Storage Buffers have not been added");
            return;
    }
    vk::BufferCreateInfo createInfo{
        .size = bufferInfo.size,
        .usage = usageFlags,
    };

    vma::AllocationCreateInfo allocationInfo{ .flags = allocationFlags, .usage = vma::MemoryUsage::eAuto };

    auto bufferAndAllocation = context->vmaAllocator.createBuffer(createInfo, allocationInfo, bufferAllocationInfo);

    buffer = bufferAndAllocation.second;
    bufferAllocation = bufferAndAllocation.first;
}

void VulkanBuffer::setData(void* data, size_t size) { copyMemory(bufferAllocationInfo.pMappedData, data, size); }

}  // namespace CAL

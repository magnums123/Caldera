#pragma once

#include <Defines.hpp>
#include <vk_mem_alloc.hpp>
#include <vk_mem_alloc_handles.hpp>

#include "VulkanDevice.hpp"

namespace CAL
{
struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    std::optional<uint32_t> computeFamily;
    std::optional<uint32_t> transferFamily;

    bool isComplete() const
    {
        return (
            graphicsFamily.has_value() && computeFamily.has_value() && transferFamily.has_value() &&
            presentFamily.has_value());
    }
};

struct VulkanContext
{
    vk::AllocationCallbacks* allocator{ nullptr };
    vk::Instance instance{ nullptr };
    vk::SurfaceKHR surface{ nullptr };
    VulkanDevice device;

    QueueFamilyIndices queueFamilyIndices;
    vk::Queue graphicsQueue{ nullptr };
    vk::Queue presentQueue{ nullptr };
    vk::Queue computeQueue{ nullptr };
    vk::Queue transferQueue{ nullptr };

    vma::Allocator vmaAllocator{ nullptr };

    ~VulkanContext() { instance.destroySurfaceKHR(); }
};

}  // namespace CAL

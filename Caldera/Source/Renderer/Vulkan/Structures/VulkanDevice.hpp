#pragma once

#include <Defines.hpp>

#include "../VulkanTypes.hpp"
#include "vulkan/vulkan.hpp"

namespace CAL
{

struct VulkanDevice
{
    VulkanDevice(const VulkanContext& context);
    ~VulkanDevice();

    vk::PhysicalDevice physicalDevice{ nullptr };
    vk::Device logicalDevice{ nullptr };

    vk::PhysicalDeviceProperties properties{};
    vk::PhysicalDeviceFeatures features{};
    vk::PhysicalDeviceMemoryProperties memoryProperties{};

    QueueFamilyIndices queueFamilyIndices{};
    vk::Queue graphicsQueue{ nullptr };
    vk::Queue presentQueue{ nullptr };
    vk::Queue computeQueue{ nullptr };
    vk::Queue transferQueue{ nullptr };

    VulkanSwapchainInfo swapchainInfo{};
};

}  // namespace CAL

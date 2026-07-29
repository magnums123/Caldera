#pragma once

#include <Defines.hpp>
#include <cstdint>
#include <optional>

namespace CAL
{
struct VulkanContext;

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

struct VulkanDevice
{
    void init(const VulkanContext& context);
    vk::PhysicalDevice physicalDevice{ nullptr };
    vk::Device logicalDevice{ nullptr };

    QueueFamilyIndices queueFamilyIndices;

    vk::Queue graphicsQueue{ nullptr };
    vk::Queue presentQueue{ nullptr };
    vk::Queue computeQueue{ nullptr };
    vk::Queue transferQueue{ nullptr };
};

}  // namespace CAL

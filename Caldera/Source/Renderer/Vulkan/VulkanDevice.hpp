#pragma once

#include <Defines.hpp>

namespace CAL
{
struct VulkanContext;

struct VulkanDevice
{
    void init(VulkanContext& context);
    vk::PhysicalDevice physicalDevice{ nullptr };
    vk::Device logicalDevice{ nullptr };
};

}  // namespace CAL

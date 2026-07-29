#pragma once

#include <Defines.hpp>

#include "VulkanDevice.hpp"

namespace CAL
{

struct VulkanContext
{
    vk::AllocationCallbacks* allocator{ nullptr };
    vk::Instance instance{ nullptr };
    vk::SurfaceKHR surface{ nullptr };
    VulkanDevice device;
};

}  // namespace CAL

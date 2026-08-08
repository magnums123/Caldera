#pragma once

#include <Defines.hpp>
#include <cstdint>
#include <vk_mem_alloc.hpp>

#include "../VulkanTypes.hpp"
#include "Core/Containers/Vector.hpp"
#include "vulkan/vulkan.hpp"

namespace CAL
{

struct VulkanSwapchain
{
    VulkanSwapchain(const VulkanContext& context, uint32_t width, uint32_t height);
    ~VulkanSwapchain();

    vk::SwapchainKHR swapchain{ nullptr };

    vk::SurfaceFormatKHR surfaceFormat;
    uint32_t maxFramesInFLight{ 0 };

    vk::Format depthImageFormat{ vk::Format::eUndefined };
    vk::Image depthImage{ nullptr };
    vk::ImageView depthImageView{ nullptr };
    vma::Allocation depthImageAllocation{ nullptr };

    Vector<vk::Image> images{};
    Vector<vk::ImageView> imageViews{};

   private:
    uint32_t width, height;

    static vk::SurfaceFormatKHR chooseBestSurfaceFormat(const Vector<vk::SurfaceFormatKHR>& formats);
    static vk::PresentModeKHR chooseBestPresentMode(const Vector<vk::PresentModeKHR>& presentModes);
};

}  // namespace CAL

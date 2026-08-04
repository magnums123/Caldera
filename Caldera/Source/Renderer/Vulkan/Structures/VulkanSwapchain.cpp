#include "Renderer/Vulkan/Structures/VulkanSwapchain.hpp"

#include "Core/Asserts.hpp"
#include "Core/Containers/Vector.hpp"
#include "Core/Logger.hpp"
#include "Renderer/Vulkan/Structures/VulkanDevice.hpp"
#include "vulkan/vulkan.hpp"

namespace CAL
{

VulkanSwapchain::VulkanSwapchain(const VulkanContext& context, uint32_t width, uint32_t height)
{
    width = width;
    height = height;

    if (width < context.device->swapchainInfo.surfaceCapabilities.minImageExtent.width ||
        width > context.device->swapchainInfo.surfaceCapabilities.maxImageExtent.width)
    {
        LOG_ERROR("Invalid Swapchain Extent Width: {}", width);
        LOG_ERROR("Using: {}", context.device->swapchainInfo.surfaceCapabilities.minImageExtent.width);
        width = context.device->swapchainInfo.surfaceCapabilities.minImageExtent.width;
    }
    if (height < context.device->swapchainInfo.surfaceCapabilities.minImageExtent.height ||
        height > context.device->swapchainInfo.surfaceCapabilities.maxImageExtent.height)
    {
        LOG_ERROR("Invalid Swapchain Extent Height: {}", height);
        LOG_ERROR("Using: {}", context.device->swapchainInfo.surfaceCapabilities.minImageExtent.height);
        height = context.device->swapchainInfo.surfaceCapabilities.minImageExtent.height;
    }

    surfaceFormat = chooseBestSurfaceFormat(context.device->swapchainInfo.surfaceFormats);
    auto presentMode = chooseBestPresentMode(context.device->swapchainInfo.presentModes);

    vk::SwapchainCreateInfoKHR swapchainCreateInfo{ .surface = context.surface,
                                                    .minImageCount =
                                                        context.device->swapchainInfo.surfaceCapabilities.minImageCount,
                                                    .imageFormat = surfaceFormat.format,
                                                    .imageColorSpace = surfaceFormat.colorSpace,
                                                    .imageExtent{ .width = width, .height = height },
                                                    .imageArrayLayers = 1,
                                                    .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
                                                    .preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity,
                                                    .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                    .presentMode = presentMode };

    swapchain = context.device->logicalDevice.createSwapchainKHR(swapchainCreateInfo);

    for (const auto& image : context.device->logicalDevice.getSwapchainImagesKHR(swapchain)) images.pushBack(image);
    imageViews.resize(images.size());

    Vector<vk::Format> possibleDepthFormats{ vk::Format::eD32Sfloat, vk::Format::eD24UnormS8Uint };
    for (auto format : possibleDepthFormats)
    {
        vk::FormatProperties formatProperties = context.device->physicalDevice.getFormatProperties(format);
        if (formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eDepthStencilAttachment)
            depthImageFormat = format;
    }

    ASSERT_MSG(depthImageFormat != vk::Format::eUndefined, "Failed to find suitable Format for Depth Image");
    vk::ImageCreateInfo depthImageCreateInfo{};
}

VulkanSwapchain::~VulkanSwapchain() {}

vk::SurfaceFormatKHR VulkanSwapchain::chooseBestSurfaceFormat(const Vector<vk::SurfaceFormatKHR>& formats)
{
    // TODO: Choose the actual Best Format
    return { .format = vk::Format::eB8G8R8A8Srgb, .colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear };
}

vk::PresentModeKHR VulkanSwapchain::chooseBestPresentMode(const Vector<vk::PresentModeKHR>& presentModes)
{
    // TODO: Choose the actual Best Format
    return vk::PresentModeKHR::eFifo;
}

}  // namespace CAL

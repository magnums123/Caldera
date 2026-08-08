#pragma once

#include <Defines.hpp>
#include <cstdint>
#include <vk_mem_alloc.hpp>

#include "Core/Containers/Vector.hpp"
#include "Core/Memory.hpp"
#include "Renderer/Vulkan/Structures/VulkanCommandBuffer.hpp"
#include "vulkan/vulkan.hpp"

namespace CAL
{
struct VulkanDevice;
struct VulkanSwapchain;

struct VulkanSwapchainInfo
{
    vk::SurfaceCapabilitiesKHR surfaceCapabilities{};
    Vector<vk::SurfaceFormatKHR> surfaceFormats{};
    Vector<vk::PresentModeKHR> presentModes{};
};

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
    Ref<VulkanDevice> device{ nullptr };

    vma::Allocator vmaAllocator{ nullptr };

    Ref<VulkanSwapchain> swapchain{};

    vk::CommandPool graphicsCommandPool{ nullptr };
    Vector<Ref<VulkanCommandBuffer>> graphicsCommandBuffers{};
};

}  // namespace CAL

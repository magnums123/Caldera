#pragma once

#include <Defines.hpp>

#include "vulkan/vulkan.hpp"

namespace CAL
{
enum class CommandBufferState
{
    NotAllocated,
    Ready,
    Recording,
    EndedRecording,
    Submitted
};

struct VulkanContext;

struct VulkanCommandBuffer
{
    VulkanCommandBuffer(const VulkanContext& context, const vk::CommandPool& commandPool, bool isPrimary);
    ~VulkanCommandBuffer();

    void begin(bool isOneTime, bool isSimultaneousUse);
    void end();

    void updateSubmitted();
    void reset();

    vk::CommandBuffer handle{ nullptr };
    CommandBufferState state{ CommandBufferState::NotAllocated };

    static VulkanCommandBuffer allocateAndBeginSingleUseCommandBuffer(
        const VulkanContext& context, vk::CommandPool commandPool);
    static void endSingleTimeCommandBuffer(VulkanCommandBuffer& commandBuffer, const vk::Queue& queue);
};

}  // namespace CAL

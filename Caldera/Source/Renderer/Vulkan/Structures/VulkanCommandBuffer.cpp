#include "VulkanCommandBuffer.hpp"

#include <cmath>

#include "Renderer/Vulkan/Structures/VulkanDevice.hpp"
#include "vulkan/vulkan.hpp"

namespace CAL
{

VulkanCommandBuffer::VulkanCommandBuffer(
    const VulkanContext& context, const vk::CommandPool& commandPool, bool isPrimary)
{
    vk::CommandBufferAllocateInfo allocInfo{ .commandPool = commandPool,
                                             .level = isPrimary ? vk::CommandBufferLevel::ePrimary
                                                                : vk::CommandBufferLevel::eSecondary,
                                             .commandBufferCount = 1 };

    handle = context.device->logicalDevice.allocateCommandBuffers(allocInfo)[0];
    state = CommandBufferState::Ready;
}

VulkanCommandBuffer::~VulkanCommandBuffer() { state = CommandBufferState::NotAllocated; }

void VulkanCommandBuffer::begin(bool isOneTime, bool isSimultaneousUse)
{
    vk::CommandBufferUsageFlags usageFlags;

    if (isOneTime) usageFlags |= vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    if (isSimultaneousUse) usageFlags |= vk::CommandBufferUsageFlagBits::eSimultaneousUse;

    vk::CommandBufferBeginInfo beginInfo{ .flags = usageFlags };

    auto res = handle.begin(&beginInfo);
    state = CommandBufferState::Recording;
}
void VulkanCommandBuffer::end()
{
    handle.end();
    state = CommandBufferState::EndedRecording;
}

void VulkanCommandBuffer::updateSubmitted() { state = CommandBufferState::Submitted; }

void VulkanCommandBuffer::reset()
{
    handle.reset();
    state = CommandBufferState::Ready;
}

VulkanCommandBuffer VulkanCommandBuffer::allocateAndBeginSingleUseCommandBuffer(
    const VulkanContext& context, vk::CommandPool commandPool)
{
    VulkanCommandBuffer buffer{ context, commandPool, true };
    buffer.begin(true, false);
    return buffer;
}

void VulkanCommandBuffer::endSingleTimeCommandBuffer(VulkanCommandBuffer& commandBuffer, const vk::Queue& queue)
{
    vk::SubmitInfo submitInfo{ .commandBufferCount = 1, .pCommandBuffers = &commandBuffer.handle };
    queue.submit(submitInfo);
    queue.waitIdle();
}

}  // namespace CAL

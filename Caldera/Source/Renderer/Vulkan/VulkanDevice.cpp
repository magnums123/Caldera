#include "Renderer/Vulkan/VulkanDevice.hpp"

#include <set>

#include "Core/Asserts.hpp"
#include "Core/Containers/Vector.hpp"
#include "Core/Logger.hpp"
#include "Renderer/Vulkan/VulkanTypes.hpp"
#include "Utility/String.hpp"
#include "vulkan/vulkan.hpp"

namespace CAL
{

// #include <vulkan/vulkan_win32.h>

void VulkanDevice::init(VulkanContext& context)
{
    auto physicalDevices = context.instance.enumeratePhysicalDevices();

    for (auto device : physicalDevices)
    {
        auto features = device.getFeatures();
        auto properties = device.getProperties();

        // TODO: Do more checks to select a PhysicalDevice that has all the required features we want to use.
        if (device.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
            physicalDevice = std::move(device);
    }

    if (!physicalDevice) physicalDevice = std::move(physicalDevices[0]);
    LOG_DEBUG("Selected Physical Device: {}", physicalDevice.getProperties().deviceName.data());

    auto queueFamilyProperties = physicalDevice.getQueueFamilyProperties();

    // First pass: Find dedicated or optimal queues
    for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
    {
        const auto& flags = queueFamilyProperties[i].queueFlags;

        // Check Graphics & Present support
        if (flags & vk::QueueFlagBits::eGraphics)
        {
            if (!context.queueFamilyIndices.graphicsFamily.has_value()) context.queueFamilyIndices.graphicsFamily = i;

            if (physicalDevice.getSurfaceSupportKHR(i, context.surface))
            {
                if (!context.queueFamilyIndices.presentFamily.has_value()) context.queueFamilyIndices.presentFamily = i;
            }
        }

        // Try finding a dedicated compute queue (Compute without Graphics)
        if ((flags & vk::QueueFlagBits::eCompute) && !(flags & vk::QueueFlagBits::eGraphics))
        {
            context.queueFamilyIndices.computeFamily = i;
        }

        // Try finding a dedicated transfer queue (Transfer without Graphics or Compute)
        if ((flags & vk::QueueFlagBits::eTransfer) && !(flags & vk::QueueFlagBits::eGraphics) &&
            !(flags & vk::QueueFlagBits::eCompute))
        {
            context.queueFamilyIndices.transferFamily = i;
        }
    }

    // Second pass: Fallbacks if dedicated queues were not found
    if (!context.queueFamilyIndices.computeFamily.has_value())
    {
        // Any queue with compute capability
        for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
        {
            if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eCompute)
            {
                context.queueFamilyIndices.computeFamily = i;
                break;
            }
        }
    }

    if (!context.queueFamilyIndices.transferFamily.has_value())
    {
        // Any queue with transfer capability (graphics & compute queues implicitly support transfer)
        for (uint32_t i = 0; i < queueFamilyProperties.size(); i++)
        {
            if (queueFamilyProperties[i].queueFlags &
                (vk::QueueFlagBits::eTransfer | vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute))
            {
                context.queueFamilyIndices.transferFamily = i;
                break;
            }
        }
    }

    ASSERT_MSG(context.queueFamilyIndices.isComplete(), "Failed to find all required queue families!");

    // 2. Deduplicate indices to build valid vk::DeviceQueueCreateInfo structs
    std::set<uint32_t> uniqueQueueFamilies = { context.queueFamilyIndices.graphicsFamily.value(),
                                               context.queueFamilyIndices.presentFamily.value(),
                                               context.queueFamilyIndices.computeFamily.value(),
                                               context.queueFamilyIndices.transferFamily.value() };

    float queuePriority = 1.0f;
    Vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        queueCreateInfos.pushBack(
            vk::DeviceQueueCreateInfo{
                .queueFamilyIndex = queueFamily, .queueCount = 1, .pQueuePriorities = &queuePriority });
    }

    Vector<const char*> extensions{ vk::KHRSwapchainExtensionName, vk::KHRExternalMemoryWin32ExtensionName };

    vk::StructureChain<
        vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features, vk::PhysicalDeviceVulkan12Features,
        vk::PhysicalDeviceVulkan13Features>
        deviceFeatureChain{ { .features{ .samplerAnisotropy = true } },
                            { .shaderDrawParameters = true },
                            { .descriptorIndexing = true,
                              .shaderSampledImageArrayNonUniformIndexing = true,
                              .descriptorBindingVariableDescriptorCount = true,
                              .runtimeDescriptorArray = true,
                              .bufferDeviceAddress = true },
                            { .synchronization2 = true, .dynamicRendering = true } };
    auto deviceFeatures = deviceFeatureChain.get<vk::PhysicalDeviceFeatures2>();

    vk::DeviceCreateInfo deviceCreateInfo{
        .pNext = &deviceFeatures,
        .queueCreateInfoCount = (uint32_t)queueCreateInfos.size(),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledExtensionCount = (uint32_t)extensions.size(),
        .ppEnabledExtensionNames = (const char* const*)extensions.data(),
        .pEnabledFeatures = nullptr,
    };

    logicalDevice = physicalDevice.createDevice(deviceCreateInfo, context.allocator);
}
}  // namespace CAL

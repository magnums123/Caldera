#include "Renderer/Vulkan/VulkanDevice.hpp"

#include "Core/Asserts.hpp"
#include "Core/Containers/Vector.hpp"
#include "Core/Logger.hpp"
#include "Renderer/Vulkan/VulkanTypes.hpp"
#include "Utility/String.hpp"
#include "vulkan/vulkan.hpp"

namespace CAL
{
void VulkanDevice::init(const VulkanContext& context)
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

    for (size_t i = 0; i < queueFamilyProperties.size(); i++)
    {
        if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
        {
            if (physicalDevice.getSurfaceSupportKHR(i, context.surface)) queueFamilyIndices.graphicsFamily = i;
        }
        if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eCompute) &&
            !(!!(queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) ||
              !!(queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eTransfer)))
            queueFamilyIndices.computeFamily = i;
        if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eTransfer) &&
            !(!!(queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) ||
              !!(queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eCompute)))
            queueFamilyIndices.transferFamily = i;
    }

    if (queueFamilyIndices.graphicsFamily.has_value() && !queueFamilyIndices.isComplete())
    {
        if (!queueFamilyIndices.presentFamily.has_value())
            queueFamilyIndices.presentFamily = queueFamilyIndices.graphicsFamily;
        if (!queueFamilyIndices.computeFamily.has_value())
            queueFamilyIndices.computeFamily = queueFamilyIndices.graphicsFamily;
        if (!queueFamilyIndices.transferFamily.has_value())
            queueFamilyIndices.transferFamily = queueFamilyIndices.graphicsFamily;
        LOG_WARN("Unique Compute or Transfer Queue not found");
    }

    ASSERT_MSG(queueFamilyIndices.isComplete(), "Invalid Queue Family Indices");

    float queuePriorities{ 1.f };

    // clang-format off
    Vector<vk::DeviceQueueCreateInfo> queueCreateInfos
    {
         vk::DeviceQueueCreateInfo{ .queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(), .queueCount = 1, .pQueuePriorities = &queuePriorities },
         vk::DeviceQueueCreateInfo{ .queueFamilyIndex = queueFamilyIndices.presentFamily.value(), .queueCount = 1, .pQueuePriorities = &queuePriorities },
         vk::DeviceQueueCreateInfo{ .queueFamilyIndex = queueFamilyIndices.computeFamily.value(), .queueCount = 1, .pQueuePriorities = &queuePriorities },
         vk::DeviceQueueCreateInfo{ .queueFamilyIndex = queueFamilyIndices.transferFamily.value(), .queueCount = 1, .pQueuePriorities = &queuePriorities}
    };
    // clang-format on

    Vector<const char*> extensions{ vk::KHRSwapchainExtensionName };

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

    graphicsQueue = logicalDevice.getQueue(queueFamilyIndices.graphicsFamily.value(), 0);
    presentQueue = logicalDevice.getQueue(queueFamilyIndices.presentFamily.value(), 0);
    computeQueue = logicalDevice.getQueue(queueFamilyIndices.computeFamily.value(), 0);
    transferQueue = logicalDevice.getQueue(queueFamilyIndices.transferFamily.value(), 0);
}
}  // namespace CAL

#include "VulkanRenderer.hpp"

#include <exception>
#include <vk_mem_alloc_structs.hpp>

#include "Core/Containers/Vector.hpp"
#include "Core/Logger.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Vulkan/VulkanDevice.hpp"
#include "Renderer/Vulkan/VulkanPlatform.hpp"
#include "Renderer/Vulkan/VulkanTypes.hpp"
#include "Utility/String.hpp"

// Add this macro definition in EXACTLY ONE .cpp file in your project:
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
static vk::detail::DynamicLoader dynamicLoader;

namespace CAL
{
VulkanRenderer::VulkanRenderer(const RendererInfo& rendererInfo) : Renderer(rendererInfo)
{
    vk::ApplicationInfo appInfo{};
    appInfo.setPApplicationName(rendererInfo.name.data());
    appInfo.setApplicationVersion(vk::makeVersion(0, 1, 0));
    appInfo.setPEngineName("Caldera Engine");
    appInfo.setEngineVersion(vk::makeVersion(0, 1, 0));
    appInfo.setApiVersion(vk::ApiVersion13);

    Vector<const char*> validationLayers{
#if defined(DEBUG)
        "VK_LAYER_KHRONOS_validation"
#endif
    };

    Vector<const char*> extensions{ vk::KHRSurfaceExtensionName,
                                    rendererInfo.platform->getRequiredExtensions().data() };

    vk::InstanceCreateInfo instanceCreateInfo{ .pApplicationInfo = &appInfo,
                                               .enabledLayerCount = (uint32_t)validationLayers.size(),
                                               .ppEnabledLayerNames = (const char* const*)validationLayers.data(),
                                               .enabledExtensionCount = (uint32_t)extensions.size(),
                                               .ppEnabledExtensionNames = extensions.data() };

    vk::detail::defaultDispatchLoaderDynamic.init(vkGetInstanceProcAddr);

    context.instance = vk::createInstance(instanceCreateInfo, context.allocator);
    vk::detail::defaultDispatchLoaderDynamic.init(context.instance);

    context.surface = platformCreateVulkanSurface(context, rendererInfo);

    context.device.init(context);

    vk::detail::defaultDispatchLoaderDynamic.init(
        context.instance, vkGetInstanceProcAddr, context.device.logicalDevice, vkGetDeviceProcAddr);

    context.graphicsQueue = context.device.logicalDevice.getQueue(context.queueFamilyIndices.graphicsFamily.value(), 0);
    context.presentQueue = context.device.logicalDevice.getQueue(context.queueFamilyIndices.presentFamily.value(), 0);
    context.computeQueue = context.device.logicalDevice.getQueue(context.queueFamilyIndices.computeFamily.value(), 0);
    context.transferQueue = context.device.logicalDevice.getQueue(context.queueFamilyIndices.transferFamily.value(), 0);

    vma::VulkanFunctions vkFunctions = vma::functionsFromDispatcher();
    vma::AllocationCreateInfo allocatorCreateInfo{
        // .flags
    };

    LOG_DEBUG("Initializing Vulkan Renderer");
}

VulkanRenderer::~VulkanRenderer()
{
    // context.instance.destroySurfaceKHR(context.surface);
    context.device = {};
}

void VulkanRenderer::drawFrame(const RenderPacket& renderPacket)
{
    if (beginFrame(renderPacket.deltaTime))
        if (!endFrame(renderPacket.deltaTime))
        {
            LOG_FATAL("VulkanRenderer::endFrame failed.");
            throw std::exception();
        }
}

bool VulkanRenderer::beginFrame(float deltaTime) { return true; }

bool VulkanRenderer::endFrame(float deltaTime)
{
    frameNumber += 1;
    return true;
}

void VulkanRenderer::resize(uint32_t width, uint32_t height)
{  // Do vulkan specific resize shi (swappchain recreation)
    this->width = width, this->height = height;
}

}  // namespace CAL

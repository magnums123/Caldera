#include "VulkanRenderer.hpp"

#include <cstdint>
#include <exception>
#include <vk_mem_alloc_enums.hpp>
#include <vk_mem_alloc_funcs.hpp>
#include <vk_mem_alloc_structs.hpp>

#include "Core/Asserts.hpp"
#include "Core/Containers/Vector.hpp"
#include "Core/Logger.hpp"
#include "Core/Memory.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Structures/Buffer.hpp"
#include "Renderer/Vulkan/Structures/VulkanDevice.hpp"
#include "Renderer/Vulkan/Structures/VulkanSwapchain.hpp"
#include "Renderer/Vulkan/VulkanPlatform.hpp"
#include "Renderer/Vulkan/VulkanTypes.hpp"
#include "Utility/String.hpp"
#include "vulkan/vulkan.hpp"

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

    ASSERT(context.instance != nullptr);

    context.surface = platformCreateVulkanSurface(context, rendererInfo);

    context.device = CreateRef<VulkanDevice>(MemoryTag::RENDERER, context);

    vk::detail::defaultDispatchLoaderDynamic.init(
        context.instance, vkGetInstanceProcAddr, context.device->logicalDevice, vkGetDeviceProcAddr);

    vma::VulkanFunctions vkFunctions = vma::functionsFromDispatcher();
    vma::AllocatorCreateInfo allocatorCreateInfo{
        .flags = vma::AllocatorCreateFlagBits::eBufferDeviceAddress,
        .physicalDevice = context.device->physicalDevice,
        .device = context.device->logicalDevice,
        .pVulkanFunctions = &vkFunctions,
        .instance = context.instance,
    };

    context.vmaAllocator = vma::createAllocator(allocatorCreateInfo);

    context.swapchain =
        CreateRef<VulkanSwapchain>(MemoryTag::RENDERER, context, rendererInfo.width, rendererInfo.height);

    vk::CommandPoolCreateInfo graphicsCommandPoolCreateInfo{
        .queueFamilyIndex = context.device->queueFamilyIndices.graphicsFamily.value()
    };
    context.graphicsCommandPool =
        context.device->logicalDevice.createCommandPool(graphicsCommandPoolCreateInfo, context.allocator);

    for (size_t i = 0; i < context.swapchain->images.size(); i++)
        context.graphicsCommandBuffers.pushBack(
            CreateRef<VulkanCommandBuffer>(Memory::MemoryTag::RENDERER, context, context.graphicsCommandPool, true));

    LOG_DEBUG("Initializing Vulkan Renderer");
}

VulkanRenderer::~VulkanRenderer() { context.device = {}; }

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
{
    this->width = width, this->height = height;

    context.device->swapchainInfo.surfaceCapabilities =
        context.device->physicalDevice.getSurfaceCapabilitiesKHR(context.surface);

    context.device->logicalDevice.waitIdle();
    context.swapchain.reset();
    context.swapchain = CreateRef<VulkanSwapchain>(Memory::MemoryTag::RENDERER, context, width, height);
}

Ref<Buffer> VulkanRenderer::createVertexBuffer(void* data, size_t vertexCount)
{
    BufferInfo bufferInfo{ .usage = BufferUsage::Vertex,
                           .size = vertexCount * sizeof(Vertex),
                           .backendData = &context };
    return Buffer::Create(bufferInfo);
}

Ref<Buffer> VulkanRenderer::createIndexBuffer(void* data, size_t indexCount)
{
    BufferInfo bufferInfo{ .usage = BufferUsage::Index,
                           .size = indexCount * sizeof(uint32_t),
                           .backendData = &context };
    return Buffer::Create(bufferInfo);
}

}  // namespace CAL

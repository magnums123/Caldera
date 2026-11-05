#include "VulkanRenderer.hpp"

#include <cstdint>
#include <exception>
#include <limits>
#include <vk_mem_alloc.hpp>

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

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
static vk::detail::DynamicLoader dynamicLoader;

namespace CAL
{
VulkanRenderer::VulkanRenderer(const RendererInfo &rendererInfo) : Renderer(rendererInfo)
{
  vk::ApplicationInfo appInfo{};
  appInfo.setPApplicationName(rendererInfo.name.data());
  appInfo.setApplicationVersion(vk::makeVersion(0, 1, 0));
  appInfo.setPEngineName("Caldera Engine");
  appInfo.setEngineVersion(vk::makeVersion(0, 1, 0));
  appInfo.setApiVersion(vk::ApiVersion13);

  Vector<const char *> validationLayers{
#if defined(DEBUG)
      "VK_LAYER_KHRONOS_validation"
#endif
  };

  Vector<const char *> extensions{vk::KHRSurfaceExtensionName,
                                  platformGetRequiredExtensions().data()};

  vk::InstanceCreateInfo instanceCreateInfo{.pApplicationInfo = &appInfo,
                                            .enabledLayerCount = (uint32_t)validationLayers.size(),
                                            .ppEnabledLayerNames =
                                                (const char *const *)validationLayers.data(),
                                            .enabledExtensionCount = (uint32_t)extensions.size(),
                                            .ppEnabledExtensionNames = extensions.data()};

  vk::detail::defaultDispatchLoaderDynamic.init(vkGetInstanceProcAddr);

  context.instance = vk::createInstance(instanceCreateInfo, context.allocator);
  vk::detail::defaultDispatchLoaderDynamic.init(context.instance);

  ASSERT(context.instance != nullptr);

  context.surface = platformCreateVulkanSurface(context, rendererInfo);

  context.device = CreateRef<VulkanDevice>(MemoryTag::RENDERER, context);

  vk::detail::defaultDispatchLoaderDynamic.init(context.instance, vkGetInstanceProcAddr,
                                                context.device->logicalDevice, vkGetDeviceProcAddr);

  vma::VulkanFunctions vkFunctions = vma::functionsFromDispatcher();
  vma::AllocatorCreateInfo allocatorCreateInfo{
      .flags = vma::AllocatorCreateFlagBits::eBufferDeviceAddress,
      .physicalDevice = context.device->physicalDevice,
      .device = context.device->logicalDevice,
      .pVulkanFunctions = &vkFunctions,
      .instance = context.instance,
  };

  context.vmaAllocator = vma::createAllocator(allocatorCreateInfo);

  context.swapchain = CreateRef<VulkanSwapchain>(MemoryTag::RENDERER, context, rendererInfo.width,
                                                 rendererInfo.height);

  vk::CommandPoolCreateInfo graphicsCommandPoolCreateInfo{
      .queueFamilyIndex = context.device->queueFamilyIndices.graphicsFamily.value()};
  context.graphicsCommandPool = context.device->logicalDevice.createCommandPool(
      graphicsCommandPoolCreateInfo, context.allocator);

  vk::SemaphoreCreateInfo semaphoreCreateInfo{};
  context.frameResources.resize(context.swapchain->maxFramesInFLight);

  for (size_t i = 0; i < context.swapchain->maxFramesInFLight; i++)
  {
    context.frameResources[i].graphicsCommandBuffer = CreateRef<VulkanCommandBuffer>(
        Memory::MemoryTag::RENDERER, context, context.graphicsCommandPool, true);

    context.frameResources[i].imageAcquiredSemaphore =
        context.device->logicalDevice.createSemaphore(semaphoreCreateInfo, context.allocator);
  }

  vk::StructureChain<vk::SemaphoreCreateInfo, vk::SemaphoreTypeCreateInfoKHR>
      timelineSemaphoreCreateInfo{
          semaphoreCreateInfo,
          vk::SemaphoreTypeCreateInfoKHR{.semaphoreType = vk::SemaphoreType::eTimeline,
                                         .initialValue = context.swapchain->maxFramesInFLight}};

  context.timelineSemaphore = context.device->logicalDevice.createSemaphore(
      timelineSemaphoreCreateInfo.get(), context.allocator);

  // Might need to be maxFrames in flight ++
  nextSignalValue = context.swapchain->maxFramesInFLight;
  LOG_DEBUG("Initializing Vulkan Renderer");
}

VulkanRenderer::~VulkanRenderer() { context.device = {}; }

void VulkanRenderer::drawFrame(const RenderPacket &renderPacket)
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
  const uint32_t frameResourceIndex = frameNumber++ % context.swapchain->maxFramesInFLight;
  const uint64_t signalValue = nextSignalValue++;
  const uint64_t waitValue = signalValue - context.swapchain->maxFramesInFLight;

  vk::SemaphoreWaitInfo waitInfo{
      .semaphoreCount = 1, .pSemaphores = &context.timelineSemaphore, .pValues = &waitValue};

  auto res =
      context.device->logicalDevice.waitSemaphores(waitInfo, std::numeric_limits<uint64_t>::max());

  FrameResources &frameResource = context.frameResources[frameResourceIndex];
  // Tutorial uses a command pool per frame, so resetting when using only one for graphics might
  // cause issues(Like resetting while work is still beign done/submitted for another frame)
  context.device->logicalDevice.resetCommandPool(context.graphicsCommandPool);

  vk::Semaphore &imageAcquiredSemaphore =
      context.frameResources[frameResourceIndex].imageAcquiredSemaphore;

  uint32_t imageIndex{0};
  res = context.device->logicalDevice.acquireNextImageKHR(
      context.swapchain->swapchain, std::numeric_limits<uint64_t>::max(), imageAcquiredSemaphore,
      nullptr, &imageIndex);

  if (res == vk::Result::eErrorOutOfDateKHR)
  {
    recreateSwapchain();
    return true;
  }
  else if (res == vk::Result::eSuboptimalKHR)
    recreateSwapchain();

  frameResource.graphicsCommandBuffer->begin(true, false);
  frameNumber++;
  return true;
}

void VulkanRenderer::resize(uint32_t width, uint32_t height)
{
  this->width = width, this->height = height;
  recreateSwapchain();
}

Ref<Buffer> VulkanRenderer::createVertexBuffer(void *data, size_t vertexCount)
{
  BufferInfo bufferInfo{
      .usage = BufferUsage::Vertex, .size = vertexCount * sizeof(Vertex), .backendData = &context};
  return Buffer::Create(bufferInfo);
}

Ref<Buffer> VulkanRenderer::createIndexBuffer(void *data, size_t indexCount)
{
  BufferInfo bufferInfo{
      .usage = BufferUsage::Index, .size = indexCount * sizeof(uint32_t), .backendData = &context};
  return Buffer::Create(bufferInfo);
}

void VulkanRenderer::recreateSwapchain()
{

  context.device->swapchainInfo.surfaceCapabilities =
      context.device->physicalDevice.getSurfaceCapabilitiesKHR(context.surface);

  context.device->logicalDevice.waitIdle();
  context.swapchain.reset();
  context.swapchain =
      CreateRef<VulkanSwapchain>(Memory::MemoryTag::RENDERER, context, width, height);
}

} // namespace CAL

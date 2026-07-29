#include "VulkanRenderer.hpp"

#include <cstdint>
#include <exception>

#include "Core/Containers/Vector.hpp"
#include "Core/Logger.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Vulkan/VulkanDevice.hpp"
#include "Renderer/Vulkan/VulkanPlatform.hpp"
#include "Utility/String.hpp"

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

    context.instance = vk::createInstance(instanceCreateInfo, context.allocator);

    context.surface = platformCreateVulkanSurface(context, rendererInfo);

    context.device.init(context);

    LOG_DEBUG("Initializing Vulkan Renderer");
}

VulkanRenderer::~VulkanRenderer() {}

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

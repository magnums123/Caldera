#include <Defines.hpp>

#include "Renderer.hpp"
#include "Vulkan/VulkanRenderer.hpp"

namespace CAL
{

Ref<Renderer> Renderer::Create(const RendererInfo& rendererInfo)
{
    switch (rendererInfo.backendType)
    {
        case RendererBackendType::Vulkan:
        {
            return CreateRef<VulkanRenderer>(VulkanRenderer(rendererInfo));
        }
        default:
            return nullptr;
    }
}

}  // namespace CAL

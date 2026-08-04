#include <Defines.hpp>

#include "Core/Memory.hpp"
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
            return std::move(CreateRef<VulkanRenderer>(MemoryTag::RENDERER, rendererInfo));
            // return std::move(CreateRef<VulkanRenderer>(Memory::MemoryTag::RENDERER, VulkanRenderer(rendererInfo)));
        }
        default:
            return nullptr;
    }
}

}  // namespace CAL

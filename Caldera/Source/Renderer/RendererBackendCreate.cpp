#include <Defines.hpp>
#include <utility>

#include "Core/Memory.hpp"
// #include "Renderer.hpp"
#include "Renderer/Structures/Buffer.hpp"
#include "Renderer/Vulkan/Structures/VulkanBuffer.hpp"
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
        }
        default:
            return nullptr;
    }
}

Ref<Buffer> Buffer::Create(const BufferInfo& bufferInfo)
{
    switch (bufferInfo.backendType)
    {
        case RendererBackendType::Vulkan:
            return std::move(CreateRef<VulkanBuffer>(MemoryTag::RENDERER, bufferInfo));
        default:
            return nullptr;
    }
}

}  // namespace CAL

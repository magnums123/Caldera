#pragma once

#include <Defines.hpp>

#include "Renderer/Renderer.hpp"
#include "Renderer/Vulkan/VulkanTypes.hpp"

namespace CAL
{

class VulkanRenderer : public Renderer
{
   public:
    VulkanRenderer(const RendererInfo& rendererInfo);
    ~VulkanRenderer() override;

    void drawFrame(const RenderPacket& packet) override;
    void resize(uint32_t width, uint32_t height) override;

   private:
    VulkanContext context{};
    bool beginFrame(float deltaTime) override;
    bool endFrame(float deltaTime) override;
};

}  // namespace CAL

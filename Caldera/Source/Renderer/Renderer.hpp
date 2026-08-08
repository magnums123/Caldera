#pragma once

#include <Defines.hpp>
#include <cstdint>

#include "Core/Memory.hpp"
#include "Renderer/Structures/Buffer.hpp"
#include "RendererTypes.hpp"
#include "Utility/String.hpp"

namespace CAL
{
enum class RendererBackendType
{
    Vulkan,
    OpenGL,
    DirectX
};

class Renderer
{
   public:
    Renderer(const RendererInfo& rendererInfo) : width(rendererInfo.width), height(rendererInfo.height) {}
    virtual ~Renderer() = default;

    inline uint32_t getWidth() const { return width; }
    inline uint32_t getHeight() const { return height; }

    virtual void drawFrame(const RenderPacket& packet) = 0;
    virtual void resize(uint32_t width, uint32_t height) = 0;

    virtual Ref<Buffer> createVertexBuffer(void* data, size_t vertexCount) = 0;
    virtual Ref<Buffer> createIndexBuffer(void* data, size_t indexCount) = 0;

    static Ref<Renderer> Create(const RendererInfo& rendererInfo);

   protected:
    String appName;
    uint32_t width, height;
    uint64_t frameNumber{};

    virtual bool beginFrame(float deltaTime) = 0;
    virtual bool endFrame(float deltaTime) = 0;
};

}  // namespace CAL

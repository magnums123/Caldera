#pragma once

#include <Defines.hpp>
#include <cstdint>

#include "Core/Memory.hpp"
#include "Platform/Platform.hpp"
#include "Utility/String.hpp"
#include "Window/Window.hpp"

namespace CAL
{
enum class RendererBackendType
{
    Vulkan,
    OpenGL,
    DirectX
};

struct RenderPacket
{
    float deltaTime;
};

struct RendererInfo
{
    uint32_t width, height;
    StringView name;
    RendererBackendType backendType;
    Ref<Platform>& platform;
    Ref<Window>& window;
};

class Renderer
{
   public:
    Renderer(const RendererInfo& rendererInfo) {}
    virtual ~Renderer() = default;

    inline uint32_t getWidth() const { return width; }
    inline uint32_t getHeight() const { return height; }

    virtual void drawFrame(const RenderPacket& packet) = 0;
    virtual void resize(uint32_t width, uint32_t height) = 0;

    static Ref<Renderer> Create(const RendererInfo& rendererInfo);

   protected:
    String appName;
    uint32_t width, height;
    uint64_t frameNumber{};

    virtual bool beginFrame(float deltaTime) = 0;
    virtual bool endFrame(float deltaTime) = 0;
};

}  // namespace CAL

#pragma once

#include <Defines.hpp>

#include "Platform/Platform.hpp"
#include "Renderer/Mesh/Mesh.hpp"
#include "Window/Window.hpp"

namespace CAL
{

struct RenderPacket
{
    float deltaTime;
    Vector<Ref<MeshAsset>> meshes;
};

struct RendererInfo
{
    uint32_t width, height;
    StringView name;
    RendererBackendType backendType;
    Ref<Platform>& platform;
    Ref<Window>& window;
};

}  // namespace CAL

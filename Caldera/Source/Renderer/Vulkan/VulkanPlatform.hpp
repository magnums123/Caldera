#pragma once

#include <Defines.hpp>

#include "Renderer/Renderer.hpp"
#include "Utility/String.hpp"
#include "Renderer/Vulkan/VulkanTypes.hpp"

namespace CAL
{

StringView platformGetRequiredExtensions();
vk::SurfaceKHR platformCreateVulkanSurface(VulkanContext& context,
                                           const RendererInfo& rendererInfo);

} // namespace CAL

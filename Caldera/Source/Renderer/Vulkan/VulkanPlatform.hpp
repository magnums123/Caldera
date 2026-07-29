#pragma once

#include <Defines.hpp>

#include "Renderer/Renderer.hpp"
#include "Renderer/Vulkan/VulkanTypes.hpp"

namespace CAL
{

vk::SurfaceKHR platformCreateVulkanSurface(VulkanContext& context, const RendererInfo& rendererInfo);

}  // namespace CAL

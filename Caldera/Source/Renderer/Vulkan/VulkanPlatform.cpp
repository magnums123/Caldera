#include "VulkanPlatform.hpp"

namespace CAL
{

#if defined(_WIN32)
#define WIN32_MEAN_AND_LEAN
// clang-format off
// #include <windows.h>
// clang-format on
#endif

#include "Renderer/Vulkan/VulkanTypes.hpp"

vk::SurfaceKHR platformCreateVulkanSurface(VulkanContext& context, const RendererInfo& rendererInfo)
{
#if defined(_WIN32)
    vk::Win32SurfaceCreateInfoKHR surfaceCreateInfo{ .hinstance = (HINSTANCE)rendererInfo.window->getInternalState(),
                                                     .hwnd = (HWND)rendererInfo.window->getHandle() };

    vk::SurfaceKHR surface(nullptr);
    auto res = context.instance.createWin32SurfaceKHR(&surfaceCreateInfo, nullptr, &surface);
    return std::move(surface);
#endif

    return nullptr;
}
}  // namespace CAL

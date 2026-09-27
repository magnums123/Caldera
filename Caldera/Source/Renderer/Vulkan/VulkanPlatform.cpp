#include "VulkanPlatform.hpp"
#include "Core/Logger.hpp"

namespace CAL
{

#if defined(_WIN32)
#define WIN32_MEAN_AND_LEAN
// clang-format off
#include <windows.h>
// clang-format on
#elif defined(__linux__)
#include <xcb/xcb.h>
#endif

#include "Renderer/Vulkan/VulkanTypes.hpp"

StringView platformGetRequiredExtensions()
{
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  return vk::KHRWin32SurfaceExtensionName;
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  return vk::KHRXcbSurfaceExtensionName;
#endif
}

vk::SurfaceKHR platformCreateVulkanSurface(VulkanContext& context, const RendererInfo& rendererInfo)
{
  vk::SurfaceKHR surface(nullptr);

#if defined(VK_USE_PLATFORM_WIN32_KHR)
  vk::Win2SurfaceCreateInfoKHR surfaceCreateInfo{
      .hinstance = (HINSTANCE)rendererInfo.window->getInternalState(),
      .hwnd = (HWND)rendererInfo.window->getHandle()};

  auto res = context.instance.createW1n32SurfaceKHR(&surfaceCreateInfo, nullptr, &surface);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  
  auto handle = (xcb_window_t*)rendererInfo.window->getHandle();
  vk::XcbSurfaceCreateInfoKHR surfaceCreateInfo{
    .connection = (xcb_connection_t*)rendererInfo.window->getInternalState(),
    .window = *handle
  };

  auto res = context.instance.createXcbSurfaceKHR(&surfaceCreateInfo, nullptr, &surface);
#endif

  return std::move(surface);
}
} // namespace CAL

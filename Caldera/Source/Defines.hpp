#pragma once

#include "caldera_export.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX  // Prevents min() and max() macro conflicts as well
#endif

#ifndef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif
// #define VULKAN_HPP_DEFAULT_DISPATCHER_TYPE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC_TYPE
#define VK_USE_PLATFORM_WIN32_KHR
#define VULKAN_HPP_NO_CONSTRUCTORS
#include <vulkan/vulkan.hpp>

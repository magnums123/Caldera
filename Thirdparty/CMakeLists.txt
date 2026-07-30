add_subdirectory(Vulkan-Loader)
add_subdirectory(Vulkan-Headers)
add_subdirectory(VulkanMemoryAllocator-Hpp)

target_link_libraries(VulkanMemoryAllocator-Hpp INTERFACE VulkanMemoryAllocator-Hpp GPUOpen::VulkanMemoryAllocator ${VMA_HPP_VULKAN_HEADERS})
# target_link_libraries(VulkanMemoryAllocator-Hpp PRIVATE vulkan Vulkan::Headers)

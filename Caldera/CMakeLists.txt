project(Caldera)
# Collect all source files in ${PROJECT_NAME}/Source
file(GLOB_RECURSE ENGINE_SOURCES "Source/*.cpp" "Source/*.hpp" "Source/*.h")

# 1. Build Caldera as a shared library
add_library(${PROJECT_NAME} SHARED ${ENGINE_SOURCES})

# 2. Include CMake's Export Header Generator
include(GenerateExportHeader)

# 3. Generate the export header with your custom macro
generate_export_header(${PROJECT_NAME}
    EXPORT_MACRO_NAME CALEXPORT
)

# 4. Set up clean include paths for both Caldera and Sandbox
target_include_directories(${PROJECT_NAME} PUBLIC 
    # Points to "Caldera/Source" so Sandbox can do #include <Core.hpp> or #include "Core.hpp"
    ${CMAKE_CURRENT_SOURCE_DIR}/Source

    ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/Vulkan-Headers/include 
    
    # Points to "build/Caldera" so anyone including Caldera files can resolve #include "${PROJECT_NAME}_export.h"
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Using SYSTEM PRIVATE removes the warnings cluttering the logs
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/VulkanMemoryAllocator-Hpp/include
)

target_link_directories(${PROJECT_NAME} PRIVATE

  ${CMAKE_CURRENT_SOURCE_DIR}/../Thirdparty/Vulkan-Loader/lib 
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>:NDEBUG RELEASE>
        $<$<CONFIG:RelWithDebInfo>:NDEBUG RELEASE>
        $<$<CONFIG:MinSizeRel>:NDEBUG RELEASE>
)

# Link Vulkan
target_link_libraries(${PROJECT_NAME} PRIVATE vulkan Vulkan::Headers VulkanMemoryAllocator-Hpp::VulkanMemoryAllocator-Hpp)

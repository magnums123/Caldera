# Collect all source files in Engine/Source
file(GLOB_RECURSE ENGINE_SOURCES "Source/*.cpp" "Source/*.hpp" "Source/*.h")

# 1. Build Caldera as a shared library
add_library(Engine SHARED ${ENGINE_SOURCES})

# 2. Include CMake's Export Header Generator
include(GenerateExportHeader)

# 3. Generate the export header with your custom macro
generate_export_header(Engine
    EXPORT_MACRO_NAME CALEXPORT
)

# 4. Set up clean include paths for both Engine and Sandbox
target_include_directories(Engine PUBLIC 
    # Points to "Engine/Source" so Sandbox can do #include <Core.hpp> or #include "Core.hpp"
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    
    # Points to "build/Engine" so anyone including Engine files can resolve #include "engine_export.h"
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Link Vulkan
target_link_libraries(Engine PUBLIC Vulkan::Vulkan)

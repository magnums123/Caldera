# Collect all source files in Sandbox/Source
file(GLOB_RECURSE SANDBOX_SOURCES "Source/*.cpp" "Source/*.hpp" "Source/*.h")

# Define the executable
add_executable(Sandbox ${SANDBOX_SOURCES})

# Link against our Engine library
target_link_libraries(Sandbox PRIVATE Engine)

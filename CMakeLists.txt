cmake_minimum_required(VERSION 3.20)
project(Caldera LANGUAGES CXX)

# Enforce C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Vulkan SDK
find_package(Vulkan REQUIRED)

# Include subdirectories
add_subdirectory(Engine)
add_subdirectory(Sandbox)

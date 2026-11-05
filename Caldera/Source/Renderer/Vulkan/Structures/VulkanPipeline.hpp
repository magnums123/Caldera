#pragma once

#include "Defines.hpp"
#include "Renderer/Vulkan/VulkanTypes.hpp"

#include <filesystem>

namespace CAL {

struct VulkanPipeline {
  VulkanPipeline(const std::filesystem::path &vertexShaderPath,
                 const std::filesystem::path &fragmentShaderPath,
                 VulkanContext &context);
  ~VulkanPipeline();

private:
  vk::ShaderModule vertexShaderModule{nullptr};
  vk::ShaderModule fragmentShaderModule{nullptr};
  vk::PipelineLayout pipelineLayout{nullptr};
  vk::Pipeline pipeline{nullptr};
};

} // namespace CAL

#include "VulkanPipeline.hpp"
#include "Renderer/Shader/Shader.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"
#include "vulkan/vulkan.hpp"

namespace CAL {

VulkanPipeline::VulkanPipeline(const std::filesystem::path &vertexShaderPath,
                               const std::filesystem::path &fragmentShaderPath,
                               VulkanContext &context) {
  auto vertexShader = context.shaderCompiler.compileShader(vertexShaderPath,
                                                           ShaderType::Vertex);
  auto fragmentShader = context.shaderCompiler.compileShader(
      fragmentShaderPath, ShaderType::Fragment);

  vk::ShaderModuleCreateInfo vertexShaderModuleCreateInfo{
      .codeSize = vertexShader.byteCode.size() * sizeof(uint32_t),
      .pCode = vertexShader.byteCode.data(),
  };
  vk::ShaderModuleCreateInfo fragmentShaderModuleCreateInfo{
      .codeSize = fragmentShader.byteCode.size() * sizeof(uint32_t),
      .pCode = fragmentShader.byteCode.data(),
  };
  vertexShaderModule = context.device->logicalDevice.createShaderModule(
      vertexShaderModuleCreateInfo);
  fragmentShaderModule = context.device->logicalDevice.createShaderModule(
      fragmentShaderModuleCreateInfo);

  vk::PipelineLayoutCreateInfo layoutCreateInfo{.setLayoutCount = 0,
                                                .pushConstantRangeCount = 0};
  pipelineLayout =
      context.device->logicalDevice.createPipelineLayout(layoutCreateInfo);

  Vector<vk::PipelineShaderStageCreateInfo> shaderStages{
      {.stage = vk::ShaderStageFlagBits::eVertex,
       .module = vertexShaderModule,
       .pName = vertexShader.entryPointName.c_str()},
      {.stage = vk::ShaderStageFlagBits::eFragment,
       .module = fragmentShaderModule,
       .pName = fragmentShader.entryPointName.c_str()}};

  vk::PipelineVertexInputStateCreateInfo vertexInputState{};

  vk::PipelineInputAssemblyStateCreateInfo inputAssemblyState{
      .topology = vk::PrimitiveTopology::eTriangleStrip};

  vk::PipelineTessellationStateCreateInfo tessellationState{};

  vk::PipelineViewportStateCreateInfo viewportState{.viewportCount = 1,
                                                    .pViewports = nullptr,
                                                    .scissorCount = 1,
                                                    .pScissors = nullptr};

  vk::PipelineRasterizationStateCreateInfo rasterizationState{
      .polygonMode = vk::PolygonMode::eFill,
      .cullMode = vk::CullModeFlagBits::eBack,
      .frontFace = vk::FrontFace::eCounterClockwise,
      .lineWidth = 1.f,
  };

  vk::PipelineMultisampleStateCreateInfo multisampleState{
      .rasterizationSamples = vk::SampleCountFlagBits::e1,
      .sampleShadingEnable = false,
      .alphaToOneEnable = false,
  };

  vk::PipelineDepthStencilStateCreateInfo depthStencilState{
      .depthTestEnable = true,
      .depthWriteEnable = true,
      .depthCompareOp = vk::CompareOp::eLess,
      .stencilTestEnable = false};

  vk::PipelineColorBlendAttachmentState colorBlendAttachmentState{
      .blendEnable = false,
      .colorWriteMask =
          vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};

  vk::PipelineColorBlendStateCreateInfo colorBlendState{
      .attachmentCount = 1, .pAttachments = &colorBlendAttachmentState};

  Vector<vk::DynamicState> dynamicStates{vk::DynamicState::eViewport,
                                         vk::DynamicState::eScissor};

  vk::PipelineDynamicStateCreateInfo dynamicState{
      .dynamicStateCount = dynamicStates.size(),
      .pDynamicStates = dynamicStates.data()};

  vk::StructureChain<vk::GraphicsPipelineCreateInfo,
                     vk::PipelineRenderingCreateInfo>
      pipelineCreateInfo{
          vk::GraphicsPipelineCreateInfo{
              .stageCount = shaderStages.size(),
              .pStages = shaderStages.data(),
              .pVertexInputState = &vertexInputState,
              .pInputAssemblyState = &inputAssemblyState,
              .pTessellationState = nullptr,
              .pViewportState = &viewportState,
              .pRasterizationState = &rasterizationState,
              .pMultisampleState = &multisampleState,
              .pDepthStencilState = &depthStencilState,
              .pColorBlendState = &colorBlendState,
              .pDynamicState = &dynamicState,
              .layout = pipelineLayout},
          vk::PipelineRenderingCreateInfo{
              .colorAttachmentCount = 1,
              .pColorAttachmentFormats =
                  &context.swapchain->surfaceFormat.format,
              .depthAttachmentFormat = context.swapchain->depthImageFormat}};

  auto res = context.device->logicalDevice.createGraphicsPipeline(
      nullptr, pipelineCreateInfo.get(), context.allocator);

  pipeline = res.value;
}

VulkanPipeline::~VulkanPipeline() {}
} // namespace CAL

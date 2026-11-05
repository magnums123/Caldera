#pragma once

#include "Defines.hpp"
#include "Shader.hpp"
#include <filesystem>

#include <shaderc/shaderc.hpp>

namespace CAL {

class ShaderCompiler {
public:
  ShaderCompiler() {
#if defined(NDEBUG)
    options.SetOptimizationLevel(shaderc_optimization_level_performance);
#endif
  }
  Shader compileShader(const std::filesystem::path &filePath,
                       ShaderType shaderType) const;

private:
  shaderc::Compiler compiler;
  shaderc::CompileOptions options;
};

} // namespace CAL

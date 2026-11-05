#include "ShaderCompiler.hpp"
#include "Core/Logger.hpp"
#include "Utility/FileReader.hpp"

namespace CAL {
Shader ShaderCompiler::compileShader(const std::filesystem::path &path,
                                     ShaderType shaderType) const {
  String shaderSource = Utility::readTextFile(path);
  String entryPointName;
  shaderc_shader_kind shaderKind;

  switch (shaderType) {
  case ShaderType::Vertex: {
    entryPointName = "vertmain";
    shaderKind = shaderc_vertex_shader;
  } break;
  case ShaderType::Fragment: {
    entryPointName = "fragmain";
    shaderKind = shaderc_fragment_shader;
  } break;
  case ShaderType::Compute: {
    entryPointName = "main";
    shaderKind = shaderc_compute_shader;
  } break;
  }

  auto compilationResult = compiler.CompileGlslToSpv(
      shaderSource, shaderKind, path.string().c_str(), options);

  if (compilationResult.GetCompilationStatus() !=
      shaderc_compilation_status_success) {
    LOG_ERROR("Failed to load Shader: {}", path.string().c_str());
    LOG_ERROR("Shaderc Error: {}", compilationResult.GetErrorMessage());
  }

  Shader shader{};
  shader.path = path;
  shader.type = shaderType;
  shader.entryPointName = entryPointName;
  shader.byteCode = {compilationResult.cbegin(), compilationResult.cend()};

  return shader;
}
} // namespace CAL

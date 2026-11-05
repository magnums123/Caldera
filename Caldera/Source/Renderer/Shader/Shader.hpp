#pragma once

#include "Defines.hpp"
#include "Utility/String.hpp"

#include <cstdint>
#include <filesystem>
#include <vector>

namespace CAL {
enum class ShaderType { Vertex, Fragment, Compute };
struct Shader {
  ShaderType type;
  std::filesystem::path path;
  String entryPointName;
  std::vector<uint32_t> byteCode;
};

} // namespace CAL

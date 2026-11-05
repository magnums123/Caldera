#pragma once

#include "Defines.hpp"
#include "Utility/String.hpp"

#include <filesystem>

namespace CAL::Utility
{

String readTextFile(const std::filesystem::path& filePath);
}

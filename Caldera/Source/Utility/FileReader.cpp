#include "FileReader.hpp"

#include "Core/Logger.hpp"

#include <fstream>
#include <sstream>

namespace CAL::Utility
{
String readTextFile(const std::filesystem::path& filePath)
{
  std::ifstream fs(filePath.string().c_str(), std::ios::in);

  if (!fs.is_open())
  {
    LOG_ERROR("Failed to open file {}", filePath.string().c_str());
    return "";
  }

  std::stringstream buffer;
  buffer << fs.rdbuf();

  fs.close();

  return buffer.str();
}
} // namespace CAL::Utility

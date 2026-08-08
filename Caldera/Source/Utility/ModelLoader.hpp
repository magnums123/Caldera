#pragma once

#include <tiny_obj_loader.h>

#include <Defines.hpp>
#include <filesystem>

#include "Renderer/Mesh/Mesh.hpp"

namespace CAL::Utility
{

Mesh loadMesh(std::filesystem::path path);

}

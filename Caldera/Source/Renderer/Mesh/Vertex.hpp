#pragma once

#include <Defines.hpp>

#include "Core/Math/Vector2.hpp"
#include "Core/Math/Vector3.hpp"

namespace CAL
{

struct Vertex
{
    Math::Vector3f Position;
    Math::Vector3f Normal;
    Math::Vector2f UV;
};

}  // namespace CAL

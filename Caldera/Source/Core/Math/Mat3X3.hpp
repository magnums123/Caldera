#pragma once

#include <Defines.hpp>

#include "Vector4.hpp"

namespace CAL::Math
{
struct mat3x3f
{
    Vector4f x_axis;
    Vector4f y_axis;
    Vector4f z_axis;
};

struct mat3x3d
{
    Vector4d x_axis;
    Vector4d y_axis;
    Vector4d z_axis;
};

}  // namespace CAL::Math

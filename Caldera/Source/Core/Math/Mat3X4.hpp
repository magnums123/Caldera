#pragma once

#include <Defines.hpp>

#include "Vector4.hpp"

namespace CAL::Math
{
struct mat3x4f
{
    Vector4f x_axis;
    Vector4f y_axis;
    Vector4f z_axis;
    Vector4f w_axis;
};

struct mat3x4d
{
    Vector4d x_axis;
    Vector4d y_axis;
    Vector4d z_axis;
    Vector4d w_axis;
};

}  // namespace CAL::Math

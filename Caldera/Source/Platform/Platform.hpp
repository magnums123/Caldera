#pragma once

#include <Defines.hpp>
#include <cstdint>

#include "Core/Memory.hpp"

#include "Utility/String.hpp"

namespace CAL
{

class Platform
{
  public:
  Platform() {}
  virtual ~Platform() = default;

  virtual float getAbsoluteTime() const = 0;
  virtual void sleep(uint64_t ms) const = 0;

  static Ref<Platform> Create();
};
} // namespace CAL

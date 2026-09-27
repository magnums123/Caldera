
#pragma once

#include "Utility/String.hpp"

#if defined(_WIN32)
// clang-format off
// clang-format on

#include "Platform/Platform.hpp"

namespace CAL
{
class Win32Platform : public Platform
{
  public:
  Win32Platform();
  ~Win32Platform() override;

  float getAbsoluteTime() const override;
  void sleep(uint64_t ms) const override;
};

#endif
} // namespace CAL

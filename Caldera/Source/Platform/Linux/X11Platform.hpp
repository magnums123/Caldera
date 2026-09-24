#pragma once

#include "Platform/Platform.hpp"

#include "Utility/String.hpp"

namespace CAL
{
class X11Platform : public Platform
{
  public:
  X11Platform();
  ~X11Platform() override;

  StringView getRequiredExtensions() override;

  float getAbsoluteTime() const override;
  void sleep(uint64_t ms) const override;
};

} // namespace CAL

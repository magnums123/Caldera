#pragma once

#include "Core/AssetManager.hpp"
#include "Defines.hpp"

namespace CAL
{

class Game
{
   public:
    Game() {}
    virtual ~Game() = default;

    virtual void update(float deltaTime) = 0;
    virtual void render(float deltaTime) = 0;
    // virtual void onresize(uint32_t newWidth, uint32_t newHeight) = 0;

   private:
    // AssetManager& assetManager;
};

}  // namespace CAL

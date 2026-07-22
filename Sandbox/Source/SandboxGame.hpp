#pragma once

#include "Game/Game.hpp"

class SandboxGame : public CAL::Game
{
   public:
    SandboxGame();
    ~SandboxGame() override;

    void update(float deltaTime) override;
    void render() override;
};

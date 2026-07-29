#include "SandboxGame.hpp"

#include <EntryPoint.hpp>

std::unique_ptr<CAL::Game> CAL::createGame() { return std::make_unique<SandboxGame>(); }

SandboxGame::SandboxGame() {}

SandboxGame::~SandboxGame() {}

void SandboxGame::update(float deltaTime) {}

void SandboxGame::render(float deltaTime) {}

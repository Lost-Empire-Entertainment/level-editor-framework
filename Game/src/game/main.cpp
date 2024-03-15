//Copyright (c) <2024> <Lost Empire Entertainment>

//game
#include "core.hpp"

using Core::Game;

int main()
{
	Game::Initialize();

	Game::Run();

	Game::Shutdown();

	return 0;
}
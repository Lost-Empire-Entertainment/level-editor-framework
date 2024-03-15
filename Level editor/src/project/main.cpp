//Copyright (c) <2024> <Lost Empire Entertainment>

//level loader
#include "core.hpp"

using Core::LevelLoader;

int main()
{
	LevelLoader::Initialize();

	LevelLoader::Run();

	LevelLoader::Shutdown();

	return 0;
}
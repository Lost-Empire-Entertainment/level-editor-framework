//Copyright (c) <2024> <Lost Empire Entertainment>

//level editor
#include "core.hpp"

using Core::LevelEditor;

int main()
{
	LevelEditor::Initialize();

	LevelEditor::Run();

	LevelEditor::Shutdown();

	return 0;
}
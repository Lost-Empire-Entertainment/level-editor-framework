//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

#include <string>

using std::string;

namespace Core
{
	class LevelEditor
	{
	public:
		static inline string name = "Level editor";
		static inline string version = "1.0.0";
		static inline string docsPath;
		static inline string filesPath;

		static inline bool startedWindowLoop;

		/// <summary>
		/// The main initialization script for the level editor, this is where everything starts.
		/// </summary>
		static void Initialize();

		/// <summary>
		/// The main loop of the level editor, this is what is ran every frame.
		/// </summary>
		static void Run();

		/// <summary>
		/// Create an error popup and shut down right after user clicks Ok.
		/// </summary>
		/// <param name="errorTitle">Displayed error message title.</param>
		/// <param name="errorMessage">Displayed error message description.</param>
		static void CreateErrorPopup(
			const char* errorTitle = "NO TITLE GIVEN",
			const char* errorMessage = "NO MESSAGE GIVEN");

		/// <summary>
		/// Handles the shutdown of the level editor.
		/// </summary>
		static void Shutdown();
	};
}
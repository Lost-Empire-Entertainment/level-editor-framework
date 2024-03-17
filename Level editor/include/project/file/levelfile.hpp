//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

#include <string>
#include <map>

using std::string;
using std::map;

namespace File
{
	class LevelFile
	{
	public:
		enum class SaveType
		{
			defaultSave,
			sceneSwitch,
			shutDown
		};

		static inline bool unsavedChanges;
		static inline string levelsPath;
		static inline string currentLevelPath;
		static inline string targetLevel;

		static void CreateLevel();
		static void LoadLevel(const string& levelName);
		static void LoadGameObject(const map<string, string> obj);
		static void SaveLevel(SaveType saveType = SaveType::defaultSave, const string& targetLevel = "");
	};
}
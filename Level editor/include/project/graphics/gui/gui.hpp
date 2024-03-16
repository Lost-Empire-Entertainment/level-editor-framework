//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

//external
#include "imgui.h"

namespace Graphics::GUI
{
	class LevelEditorGUI
	{
	public:
		static inline float fontScale;

		static inline ImVec2 initialPos;
		static inline ImVec2 initialSize;

		static inline ImVec2 minSize;
		static inline ImVec2 maxSize;

		static inline bool renderUnsavedShutdownWindow;
		static inline bool renderUnsavedSceneSwitchWindow;

		static void Initialize();

		static void CustomizeImGuiStyle();

		int GetScreenWidth();
		int GetScreenHeight();
		static float GetScreenRefreshRate();

		static void Render();
		static void Shutdown();
	private:
		static void ConfirmUnsavedShutdown();

		static void ConfirmUnsavedSceneSwitch();
	};
}
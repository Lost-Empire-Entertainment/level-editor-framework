//Copyright (c) <2024> <Lost Empire Entertainment>

//external
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

//level editor
#include "gui_settings.hpp"
#include "gui.hpp"

namespace Graphics::GUI
{
	void Settings::RenderSettings()
	{
		ImGui::SetNextWindowSizeConstraints(LevelEditorGUI::minSize, LevelEditorGUI::maxSize);
		ImGui::SetNextWindowPos(LevelEditorGUI::initialPos, ImGuiCond_FirstUseEver);

		ImGuiWindowFlags windowFlags =
			ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoSavedSettings;

		if (renderSettings
			&& ImGui::Begin("Settings", NULL, windowFlags))
		{
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 40);
			if (ImGui::Button("X"))
			{
				renderSettings = false;
			}

			if (ImGui::BeginChild("SettingsChild"))
			{
				ImGui::Text("This is the settings window...");

				ImGui::EndChild();
			}

			ImGui::End();
		}
	}
}
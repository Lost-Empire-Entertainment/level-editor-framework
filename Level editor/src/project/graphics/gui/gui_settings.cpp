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
		ImGui::SetNextWindowSizeConstraints(ImVec2(600, 400), LevelEditorGUI::maxSize);
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

			ImGuiChildFlags childWindowFlags{}
			;
			ImVec2 childSize = 
				ImVec2(ImGui::GetWindowWidth() - 20, ImGui::GetWindowHeight() - 80);

			ImGui::BeginChild("SettingsChild", childSize, true, childWindowFlags);
			{
				ImVec2 buttonSize = ImVec2(175, 30);
				if (ImGui::Button("Set game levels path", buttonSize))
				{

				}
				ImGui::SameLine();
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 300);
				ImGui::Text("no path set...");

				ImGui::EndChild();
			}

			ImGui::End();
		}
	}
}
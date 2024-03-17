//Copyright (c) <2024> <Lost Empire Entertainment>

//external
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

//level editor
#include "gui_compile.hpp"
#include "gui.hpp"

namespace Graphics::GUI
{
	void Compile::RenderCompile()
	{
		float width = 550;
		float height = 300;
		ImGui::SetNextWindowSizeConstraints(ImVec2(width, height), ImVec2(width, height));
		ImGui::SetNextWindowPos(LevelEditorGUI::initialPos, ImGuiCond_FirstUseEver);

		ImGuiWindowFlags windowFlags =
			ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoSavedSettings;

		if (renderCompile
			&& ImGui::Begin("Compile", NULL, windowFlags))
		{
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 40);
			if (ImGui::Button("X"))
			{
				renderCompile = false;
			}

			if (ImGui::BeginChild("CompileChild"))
			{
				ImGui::Text("This is the compilation window...");

				ImGui::EndChild();
			}

			ImGui::End();
		}
	}
}
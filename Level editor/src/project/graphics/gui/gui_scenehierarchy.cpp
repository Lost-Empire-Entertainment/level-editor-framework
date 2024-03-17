//Copyright (c) <2024> <Lost Empire Entertainment>

#include <iostream>

//external
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

//level editor
#include "gui_scenehierarchy.hpp"
#include "gui.hpp"
#include "selectobject.hpp"
#include "input.hpp"
#include "render.hpp"

using std::cout;

using Graphics::Shape::GameObjectManager;
using Graphics::Shape::Mesh;
using Physics::Select;
using Core::Input;
using Graphics::Render;

namespace Graphics::GUI
{
	void SceneHierarchy::RenderSceneHierarchy()
	{
		glfwGetFramebufferSize(Render::window, &framebufferWidth, &framebufferHeight);

		//set window width
		int desiredWidth = 400;

		//set window height
		int windowPosY = static_cast<int>(20 * LevelEditorGUI::fontScale);
		float maxWindowHeight = static_cast<float>(framebufferHeight) - static_cast<float>(windowPosY);
		if (maxWindowHeight < 0) maxWindowHeight = 0;

		ImGui::SetNextWindowPos(
			ImVec2(0.0f, static_cast<float>(windowPosY)));
		ImGui::SetNextWindowSize(
			ImVec2(static_cast<float>(desiredWidth), static_cast<float>(maxWindowHeight)));

		ImGuiWindowFlags windowFlags =
			ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoSavedSettings;

		if (ImGui::Begin("Scene hierarchy", NULL, windowFlags))
		{
			for (const auto& obj : GameObjectManager::GetObjects())
			{
				Mesh::MeshType type = obj->GetMesh()->GetMeshType();
				if (type != Mesh::MeshType::actionTex
					&& type != Mesh::MeshType::billboard
					&& type != Mesh::MeshType::border)
				{
					DrawGameObject(obj);
				}
			}
			RightClickPopup();
			RenameGameObject();

			ImGui::End();
		}
	}

	void SceneHierarchy::DrawGameObject(shared_ptr<GameObject> obj)
	{
		ImGui::Selectable(obj->GetName().c_str(), false, ImGuiSelectableFlags_AllowItemOverlap);

		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(0))
			{
				Select::selectedObj = obj;
				Select::isObjectSelected = true;
				Input::objectAction = Input::ObjectAction::move;
			}

			if (ImGui::IsMouseClicked(1))
			{
				selectedGameObject = obj;
				ImGui::OpenPopup("rightclickpopup");
			}
		}
	}

	void SceneHierarchy::RightClickPopup()
	{
		if (ImGui::BeginPopupContextItem("rightclickpopup"))
		{
			if (ImGui::MenuItem("Rename"))
			{
				isRenaming = true;
			}

			if (ImGui::MenuItem("Delete"))
			{
				string objName = selectedGameObject->GetName();

				GameObjectManager::DestroyGameObject(selectedGameObject);

				cout << "Deleted " << objName << "\n";

				//if (!SceneFile::unsavedChanges) Render::SetWindowNameAsUnsaved(true);
			}

			ImGui::EndPopup();
		}
	}

	void SceneHierarchy::RenameGameObject()
	{
		if (isRenaming)
		{
			ImGui::OpenPopup("Rename gameobject");
			isRenaming = false;
		}

		if (ImGui::BeginPopupModal("Rename gameobject", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Enter new name:");
			ImGui::InputText("##newname", newNameBuffer, sizeof(newNameBuffer));

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				//check if the new name is not empty
				if (strlen(newNameBuffer) > 0)
				{
					selectedGameObject->SetName(newNameBuffer);
				}

				//if (!SceneFile::unsavedChanges) Render::SetWindowNameAsUnsaved(true);

				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}
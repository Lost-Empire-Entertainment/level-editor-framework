//Copyright (c) <2024> <Lost Empire Entertainment>

#include <iostream>
#include <string>
#include <filesystem>

//external
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

//level editor
#include "gui.hpp"
#include "render.hpp"
#include "core.hpp"
#include "gameobject.hpp"
#include "cube.hpp"
#include "spotlight.hpp"
#include "pointlight.hpp"
#include "selectobject.hpp"
#include "stringutils.hpp"
#include "gui_inspector.hpp"
#include "gui_scenehierarchy.hpp"
#include "gui_settings.hpp"
#include "gui_compile.hpp"
#include "configfile.hpp"

using std::cout;
using std::string;
using std::to_string;
using std::filesystem::path;
using std::filesystem::directory_iterator;
using std::filesystem::create_directory;

using Core::LevelEditor;
using Utils::StringUtils;
using Physics::Select;
using Graphics::Render;
using Graphics::Shape::Cube;
using Graphics::Shape::PointLight;
using Graphics::Shape::SpotLight;
using File::ConfigFileManager;

namespace Graphics::GUI
{
	static ImVec4 bgrColor;

	void LevelEditorGUI::Initialize()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::SetCurrentContext(ImGui::GetCurrentContext());
		ImGuiIO& io = ImGui::GetIO();

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		static string tempString = LevelEditor::docsPath + "/imgui.ini";
		const char* customConfigPath = tempString.c_str();
		io.IniFilename = customConfigPath;

		initialPos = ImVec2(100, 100);
		initialSize = ImVec2(400, 700);
		minSize = ImVec2(400, 400);

		ImGui_ImplGlfw_InitForOpenGL(Render::window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		io.Fonts->Clear();
		io.Fonts->AddFontFromFileTTF((LevelEditor::filesPath + "/fonts/coda/Coda-Regular.ttf").c_str(), 16.0f);

		bgrColor.x = Render::backgroundColor.x;
		bgrColor.y = Render::backgroundColor.y;
		bgrColor.z = Render::backgroundColor.z;
		bgrColor.w = 1.0f;

		CustomizeImGuiStyle();
	}
	void LevelEditorGUI::CustomizeImGuiStyle()
	{
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		ImGuiIO& io = ImGui::GetIO();

		style.TabRounding = 6;
		style.FramePadding = ImVec2(6, 2);
		style.ItemSpacing = ImVec2(0, 5);
		io.FontGlobalScale = fontScale;
	}

	int LevelEditorGUI::GetScreenWidth()
	{
		int width, height;
		glfwGetFramebufferSize(Render::window, &width, &height);
		return width;
	}

	int LevelEditorGUI::GetScreenHeight()
	{
		int width, height;
		glfwGetFramebufferSize(Render::window, &width, &height);
		return height;
	}

	float LevelEditorGUI::GetScreenRefreshRate()
	{
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();

		const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

		return static_cast<float>(videoMode->refreshRate);
	}

	void LevelEditorGUI::RenderTopBar()
	{
		ImGui::BeginMainMenuBar();

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				//SceneFile::SaveCurrentScene();
				ConfigFileManager::SaveConfigFile();
			}

			if (ImGui::MenuItem("New level"))
			{
				int highestFolderNumber = 1;
				for (const auto& entry : directory_iterator(LevelEditor::filesPath))
				{
					path entryPath = entry.path();
					if (is_directory(entryPath))
					{
						string folderName = entryPath.stem().string();

						if (folderName.find("Scene") != string::npos)
						{
							size_t pos = folderName.find_first_of('e', folderName.find_first_of('e') + 1);
							string result = folderName.substr(pos + 1);
							if (result != ""
								&& StringUtils::CanConvertStringToInt(result))
							{
								int number = stoi(result);
								if (number == highestFolderNumber) highestFolderNumber = ++number;
							}
						}
					}
				}
				string newFolderPath = LevelEditor::filesPath + "/Scene" + to_string(highestFolderNumber);
				create_directory(newFolderPath);
				//SceneFile::CreateNewScene(newFolderPath + "/scene.txt");
			}

			if (ImGui::MenuItem("Exit")) LevelEditor::Shutdown();

			ImGui::EndMenu();
		}

		ImGui::SameLine(50 * fontScale * 0.75f);

		if (ImGui::BeginMenu("Asset"))
		{
			if (ImGui::BeginMenu("Shape"))
			{
				if (ImGui::MenuItem("Cube"))
				{
					shared_ptr<GameObject> obj = Cube::InitializeCube();

					Select::selectedObj = obj;
					Select::isObjectSelected = true;

					unsigned int ID = obj->GetID();
					vec3 pos = obj->GetTransform()->GetPosition();
					string posX = to_string(pos.x);
					string posY = to_string(pos.y);
					string posZ = to_string(pos.z);

					string output =
						"Successfully created " + obj->GetName() +
						" with ID " + to_string(obj->GetID()) +
						" at position (" + posX + ", " + posY + ", " + posZ + ")\n";
					cout << output;

					//if (!SceneFile::unsavedChanges) Render::SetWindowNameAsUnsaved(true);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Light source"))
			{
				if (ImGui::MenuItem("Point light"))
				{
					shared_ptr<GameObject> obj = PointLight::InitializePointLight();

					Select::selectedObj = obj;
					Select::isObjectSelected = true;

					unsigned int ID = obj->GetID();
					vec3 pos = obj->GetTransform()->GetPosition();
					string posX = to_string(pos.x);
					string posY = to_string(pos.y);
					string posZ = to_string(pos.z);

					string output =
						"Successfully created " + obj->GetName() +
						" with ID " + to_string(obj->GetID()) +
						" at position (" + posX + ", " + posY + ", " + posZ + ")\n";
					cout << output;

					//if (!SceneFile::unsavedChanges) Render::SetWindowNameAsUnsaved(true);
				}
				if (ImGui::MenuItem("Spotlight"))
				{
					shared_ptr<GameObject> obj = SpotLight::InitializeSpotLight();

					Select::selectedObj = obj;
					Select::isObjectSelected = true;

					unsigned int ID = obj->GetID();
					vec3 pos = obj->GetTransform()->GetPosition();
					string posX = to_string(pos.x);
					string posY = to_string(pos.y);
					string posZ = to_string(pos.z);

					string output =
						"Successfully created " + obj->GetName() +
						" with ID " + to_string(obj->GetID()) +
						" at position (" + posX + ", " + posY + ", " + posZ + ")\n";
					cout << output;

					//if (!SceneFile::unsavedChanges) Render::SetWindowNameAsUnsaved(true);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::SameLine(110 * fontScale * 0.75f);

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::MenuItem("Settings"))
			{
				Settings::renderSettings = true;
			}

			ImGui::EndMenu();
		}

		ImGui::SameLine(180 * fontScale * 0.75f);

		if (ImGui::BeginMenu("Compile"))
		{
			Compile::renderCompile = true;

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	void LevelEditorGUI::ConfirmUnsavedShutdown()
	{

	}

	void LevelEditorGUI::ConfirmUnsavedSceneSwitch()
	{

	}

	void LevelEditorGUI::Render()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		maxSize = ImVec2(io.DisplaySize.x, io.DisplaySize.y - 200);

		RenderTopBar();

		ImGuiDockNodeFlags dockFlags =
			ImGuiDockNodeFlags_PassthruCentralNode;

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), dockFlags);

		SceneHierarchy::RenderSceneHierarchy();
		Inspector::RenderInspector();
		Settings::RenderSettings();
		Compile::RenderCompile();

		if (renderUnsavedShutdownWindow) ConfirmUnsavedShutdown();
		if (renderUnsavedSceneSwitchWindow) ConfirmUnsavedSceneSwitch();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void LevelEditorGUI::Shutdown()
	{
		//close any remaining open ImGui windows
		for (ImGuiWindow* window : ImGui::GetCurrentContext()->Windows)
		{
			if (window->WasActive)
			{
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::StyleColorsDark();
		ImGui::GetIO().IniFilename = nullptr;

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}
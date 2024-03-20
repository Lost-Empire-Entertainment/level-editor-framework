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
#include "levelfile.hpp"
#include "fileexplorer.hpp"

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
using File::LevelFile;
using File::FileExplorer;

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
				LevelFile::SaveLevel();
				ConfigFileManager::SaveConfigFile();

				if (LevelFile::unsavedChanges) Render::SetWindowNameAsUnsaved(false);
			}

			if (ImGui::MenuItem("New level"))
			{
				LevelFile::CreateLevel();
			}

			if (ImGui::MenuItem("Load level"))
			{
				string path = FileExplorer::Select();
				if (path == "") cout << "Error: Did not get path!\n\n";
				else LevelFile::LoadLevel(path);
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
					bool canPlaceAssets = LevelFile::currentLevelPath != "";
					if (!canPlaceAssets)
					{
						cout << "Error: Cannot place assets because no level is loaded!\n";
					}
					else
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

						if (!LevelFile::unsavedChanges) Render::SetWindowNameAsUnsaved(true);
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Light source"))
			{
				if (ImGui::MenuItem("Point light"))
				{
					bool canPlaceAssets = LevelFile::currentLevelPath != "";
					if (!canPlaceAssets)
					{
						cout << "Error: Cannot place assets because no level is loaded!\n";
					}
					else
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

						if (!LevelFile::unsavedChanges) Render::SetWindowNameAsUnsaved(true);
					}
				}
				if (ImGui::MenuItem("Spotlight"))
				{
					bool canPlaceAssets = LevelFile::currentLevelPath != "";
					if (!canPlaceAssets)
					{
						cout << "Error: Cannot place assets because no level is loaded!\n";
					}
					else
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

						if (!LevelFile::unsavedChanges) Render::SetWindowNameAsUnsaved(true);
					}
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
		ImGui::SetNextWindowPos(ImVec2(200, 200), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(500, 300));

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoSavedSettings;

		string title = "You have unsaved changes. Save before closing?";
		ImGui::Begin(title.c_str(), nullptr, flags);

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImVec2 windowCenter(windowPos.x + windowSize.x * 0.5f, windowPos.y + windowSize.y * 0.5f);
		ImVec2 buttonSize(120, 50);
		float buttonSpacing = 20.0f;

		ImVec2 button1Pos(
			windowSize.x * 0.3f - buttonSize.x,
			windowSize.y * 0.5f - buttonSize.y * 0.5f);
		ImVec2 button2Pos(
			windowSize.x * 0.625f - buttonSize.x,
			windowSize.y * 0.5f - buttonSize.y * 0.5f);
		ImVec2 button3Pos(
			windowSize.x * 0.7f,
			windowSize.y * 0.5f - buttonSize.y * 0.5f);

		ImGui::SetCursorPos(button1Pos);
		if (ImGui::Button("Save", buttonSize))
		{
			LevelFile::SaveLevel();
			renderUnsavedShutdownWindow = false;
		}

		ImGui::SetCursorPos(button2Pos);
		if (ImGui::Button("Don't save", buttonSize))
		{
			cout << "Closed engine without saving.\n";
			LevelFile::unsavedChanges = false;
			LevelEditor::Shutdown();
		}

		ImGui::SetCursorPos(button3Pos);
		if (ImGui::Button("Cancel", buttonSize))
		{
			cout << "Cancelled shutdown...\n";
			renderUnsavedShutdownWindow = false;
		}

		ImGui::End();
	}

	void LevelEditorGUI::ConfirmUnsavedSceneSwitch()
	{
		ImGui::SetNextWindowPos(ImVec2(200, 200), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(500, 300));

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoDocking
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoSavedSettings;

		string title = "You have unsaved changes. Save before switching scenes?";
		ImGui::Begin(title.c_str(), nullptr, flags);

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImVec2 windowCenter(windowPos.x + windowSize.x * 0.5f, windowPos.y + windowSize.y * 0.5f);
		ImVec2 buttonSize(120, 50);
		float buttonSpacing = 20.0f;

		ImVec2 button1Pos(
			windowSize.x * 0.3f - buttonSize.x,
			windowSize.y * 0.5f - buttonSize.y * 0.5f);
		ImVec2 button2Pos(
			windowSize.x * 0.625f - buttonSize.x,
			windowSize.y * 0.5f - buttonSize.y * 0.5f);
		ImVec2 button3Pos(
			windowSize.x * 0.7f,
			windowSize.y * 0.5f - buttonSize.y * 0.5f);

		ImGui::SetCursorPos(button1Pos);
		if (ImGui::Button("Save", buttonSize))
		{
			LevelFile::SaveLevel();
			LevelFile::LoadLevel(LevelFile::targetLevel);
			renderUnsavedSceneSwitchWindow = false;
		}

		ImGui::SetCursorPos(button2Pos);
		if (ImGui::Button("Don't save", buttonSize))
		{
			cout << "Switched scene without saving.\n";
			LevelFile::LoadLevel(LevelFile::targetLevel);
			renderUnsavedSceneSwitchWindow = false;
		}

		ImGui::SetCursorPos(button3Pos);
		if (ImGui::Button("Cancel", buttonSize))
		{
			cout << "Cancelled scene switch...\n";
			renderUnsavedSceneSwitchWindow = false;
		}

		ImGui::End();
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
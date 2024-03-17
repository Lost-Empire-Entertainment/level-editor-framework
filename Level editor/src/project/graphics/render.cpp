//Copyright (c) <2024> <Lost Empire Entertainment>

#include <iostream>

//external
#include "glad.h"
#include "glfw3.h"
#include "glm.hpp"
#include "matrix_transform.hpp"
#include "type_ptr.hpp"
#include "stb_image.h"

//level editor
#include "render.hpp"
#include "timemanager.hpp"
#include "core.hpp"
#include "gameobject.hpp"
#include "selectedobjectaction.hpp"
#include "selectedobjectborder.hpp"
#include "grid.hpp"
#include "gui.hpp"
#include "configfile.hpp"
#include "levelfile.hpp"

using std::cout;
using glm::perspective;

using Core::LevelEditor;
using Core::TimeManager;
using Graphics::Shape::GameObject;
using Graphics::Shape::GameObjectManager;
using Graphics::Shape::ActionTex;
using Graphics::Shape::Border;
using Graphics::Grid;
using Graphics::GUI::LevelEditorGUI;
using File::ConfigFileManager;
using File::LevelFile;

namespace Graphics
{
	Input Render::camera(Render::window);

	void Render::RenderSetup()
	{
		Render::GLFWSetup();
		Render::WindowSetup();
		Render::GladSetup();

		Render::ContentSetup();

		LevelEditorGUI::Initialize();

		TimeManager::InitializeDeltaTime();
	}

	void Render::GLFWSetup()
	{
		cout << "\nInitializing GLFW...\n";

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		cout << "GLFW initialized successfully!\n\n";
	}
	void Render::WindowSetup()
	{
		cout << "Creating window...\n";

				//create a window object holding all the windowing data
		window = glfwCreateWindow(
			windowWidth,
			windowHeight,
			(LevelEditor::name + " " + LevelEditor::version).c_str(),
			NULL,
			NULL);

		if (window == NULL)
		{
			LevelEditor::CreateErrorPopup("Initialization failed", "Failed to create GLFW window!");
		}

		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, UpdateAfterRescale);
		glfwGetWindowSize(window, &windowedWidth, &windowedHeight);
		glfwSetWindowSizeLimits(window, 1280, 720, 7680, 4320);
		glfwSwapInterval(1);

		int width, height, channels;
		string iconpath = LevelEditor::filesPath + "/icon.png";
		unsigned char* iconData = stbi_load(iconpath.c_str(), &width, &height, &channels, 4);

		GLFWimage icon{};
		icon.width = width;
		icon.height = height;
		icon.pixels = iconData;

		glfwSetWindowIcon(window, 1, &icon);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
		glfwSetScrollCallback(window, Input::ScrollCallback);
		glfwSetKeyCallback(window, Input::KeyCallback);
		glfwSetCursorPosCallback(window, Input::MouseMovementCallback);

		glfwSetWindowCloseCallback(window, [](GLFWwindow* window) { LevelEditor::Shutdown(); });

		cout << "Window created successfully!\n\n";
	}
	void Render::GladSetup()
	{
		cout << "Initializing GLAD...\n";

		//check if glad is initialized before continuing
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LevelEditor::CreateErrorPopup("Initialization failed", "Failed to initialize GLAD!");
		}

		cout << "GLAD initialized successfully!\n\n";
	}

	void Render::ContentSetup()
	{
		//enable depth testing
		glEnable(GL_DEPTH_TEST);
		//enable blending
		glEnable(GL_BLEND);
		//set blending function
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Grid::InitializeGrid();

		shared_ptr<GameObject> border = Border::InitializeBorder();
		GameObjectManager::SetBorder(border);
		GameObjectManager::AddOpaqueObject(border);

		shared_ptr<GameObject> actionTex = ActionTex::InitializeActionTex();
		GameObjectManager::SetActionTex(actionTex);
		GameObjectManager::AddTransparentObject(actionTex);

		UpdateAfterRescale(window, windowWidth, windowHeight);
	}

	void Render::UpdateAfterRescale(GLFWwindow* window, int width, int height)
	{
		//Calculate the new aspect ratio
		Input::aspectRatio = static_cast<float>(width) / static_cast<float>(height);

		//Set the viewport based on the aspect ratio
		glViewport(0, 0, width, height);

		if (LevelEditor::startedWindowLoop
			&& !LevelFile::unsavedChanges)
		{
			Render::SetWindowNameAsUnsaved(true);
		}
	}

	void Render::SetWindowNameAsUnsaved(bool state)
	{
		LevelFile::unsavedChanges = state;

		string newName = state == true
			? LevelEditor::name + " " + LevelEditor::version + "*"
			: LevelEditor::name + " " + LevelEditor::version;
		glfwSetWindowTitle(Render::window, newName.c_str());
	}

	void Render::WindowLoop()
	{
		glClearColor(
			backgroundColor.x,
			backgroundColor.y,
			backgroundColor.z,
			1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//camera transformation
		Input::ProcessKeyboardInput(window);

		//calculate the new projection matrix
		projection = perspective(
			radians(Input::fov),
			Input::aspectRatio,
			Input::nearClip,
			Input::farClip);

		//update the camera
		view = camera.GetViewMatrix();

		Grid::RenderGrid(view, projection);

		GameObjectManager::RenderAll(view, projection);

		LevelEditorGUI::Render();

		//swap the front and back buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
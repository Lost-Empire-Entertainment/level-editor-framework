//Copyright (c) <2024> <Lost Empire Entertainment>

#include <Windows.h>
#include <ShlObj.h>
#include <filesystem>
#include <iostream>

//level loader
#include "core.hpp"
#include "stringutils.hpp"
#include "render.hpp"

using std::cout;
using std::wstring;
using std::filesystem::exists;
using std::filesystem::create_directory;
using std::filesystem::path;
using std::filesystem::current_path;
using std::filesystem::copy_file;

using Utils::StringUtils;
using Graphics::Render;

namespace Core
{
	void LevelLoader::Initialize()
	{
		//
		// SET DOCUMENTS PATH
		//

		PWSTR docsFolderWidePath;
		HRESULT result = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &docsFolderWidePath);
		if (SUCCEEDED(result))
		{
			wstring wPath(docsFolderWidePath);
			CoTaskMemFree(docsFolderWidePath); //free the allocated memory

			//get the required buffer size
			int size_needed = WideCharToMultiByte(
				CP_UTF8,
				0,
				wPath.c_str(),
				static_cast<int>(wPath.length()),
				NULL,
				0,
				NULL,
				NULL);

			//convert wide string to utf-8 encoded narrow string
			string narrowPath(size_needed, 0);
			WideCharToMultiByte(
				CP_UTF8,
				0,
				wPath.c_str(),
				static_cast<int>(wPath.length()),
				&narrowPath[0],
				size_needed,
				NULL,
				NULL);

			docsPath = StringUtils::StringReplace(
				string(narrowPath.begin(), narrowPath.end()), "\\", "/") +
				"/" + name;

			cout << "Set documents path to " << docsPath << "\n";
			if (!exists(docsPath)) create_directory(docsPath);
		}
		else
		{
			string errorMessage = "Couldn't find " + name + " documents folder! Shutting down.";
			CreateErrorPopup("Path load error", errorMessage.c_str());
		}

		//
		// SET FILES PATH
		//

		path fsFilesPath = current_path().generic_string() + "/files";
		if (!exists(fsFilesPath))
		{
			CreateErrorPopup("Path load error", "Couldn't find files folder! Shutting down.");
			return;
		}
		cout << "Set files path to " << filesPath << "\n";
		filesPath = fsFilesPath.string();

		//
		// COPY config.txt TO ENGINE DOCUMENTS FOLDER IF config.txt DOESNT EXIST
		//

		if (!exists(docsPath + "/config.txt"))
		{
			copy_file(filesPath + "/template files/config.txt", docsPath + "/config.txt");
		}

		Render::RenderSetup();
	}

	void LevelLoader::Run()
	{
		startedWindowLoop = true;

		while (!glfwWindowShouldClose(Render::window))
		{
			Render::WindowLoop();
		}
	}

	void LevelLoader::CreateErrorPopup(
		const char* errorTitle,
		const char* errorMessage)
	{
		int result = MessageBoxA(nullptr, errorMessage, errorTitle, MB_ICONERROR | MB_OK);

		if (result == IDOK) Shutdown();
	}

	void LevelLoader::Shutdown()
	{
		//clean all glfw resources after program is closed
		glfwTerminate();
	}
}
//Copyright (c) <2024> <Lost Empire Entertainment>

#include <vector>
#include <memory>
#include <filesystem>
#include <fstream>
#include <iostream>

//external
#include "magic_enum.hpp"

//level editor
#include "levelfile.hpp"
#include "core.hpp"
#include "gameobject.hpp"
#include "selectobject.hpp"
#include "stringutils.hpp"
#include "render.hpp"
#include "cube.hpp"
#include "pointlight.hpp"
#include "spotlight.hpp"
#include "fileutils.hpp"

using std::ifstream;
using std::ofstream;
using std::cout;
using std::to_string;
using std::filesystem::exists;
using std::filesystem::path;
using std::filesystem::directory_iterator;

using Core::LevelEditor;
using Physics::Select;
using Graphics::Shape::GameObject;
using Graphics::Shape::GameObjectManager;
using Utils::StringUtils;
using Graphics::Render;
using Graphics::Shape::Mesh;
using Graphics::Shape::Cube;
using Graphics::Shape::PointLight;
using Graphics::Shape::SpotLight;
using Utils::FileUtils;

namespace File
{
	void LevelFile::CreateLevel()
	{
		int highestIndex = 1;
		for (const auto& entry : directory_iterator(levelsPath))
		{
			path entryPath = entry;
			string entryName = entryPath.stem().string();

			string removedPart = "Save";
			size_t pos = entryName.find(removedPart);
			if (pos != string::npos
				&& entryName.size() >= 5)
			{
				entryName.erase(pos, removedPart.length());
				if (StringUtils::CanConvertStringToInt(entryName))
				{
					int index = stoi(entryName);
					if (highestIndex <= index) highestIndex = ++index;
				}
			}
		}
		currentLevelPath = levelsPath + "/Save" + to_string(highestIndex) + ".txt";

		ofstream levelFile(currentLevelPath);

		if (!levelFile.is_open())
		{
			cout << "Error: Couldn't open level file '" << currentLevelPath << "'!\n";
			return;
		}

		levelFile.close();

		cout << "\nSuccessfully created new level '" << currentLevelPath << "'!\n";

		LoadLevel(currentLevelPath);
	}

	void LevelFile::LoadLevel(const string& levelPath)
	{
		if (!exists(levelPath))
		{
			cout << "Error: Tried to load level '" << levelPath << "' but it doesn't exist!\n";
			return;
		}

		Select::isObjectSelected = false;
		Select::selectedObj = nullptr;

		vector<shared_ptr<GameObject>> objects = GameObjectManager::GetObjects();
		if (objects.size() != 0)
		{
			for (const auto& obj : objects)
			{
				GameObjectManager::DestroyGameObject(obj);
			}
		}

		ifstream levelFile(levelPath);
		if (!levelFile.is_open())
		{
			cout << "Error: Failed to open level file '" << levelPath << "'!\n\n";
			return;
		}

		string line;
		map<string, string> obj;
		while (getline(levelFile, line))
		{
			if (!line.empty()
				&& line.find(":") != string::npos)
			{
				vector<string> splitLine = StringUtils::Split(line, ':');
				string type = splitLine[0];
				string value = splitLine[1];

				//remove one space in front of value if it exists
				if (value[0] == ' ') value.erase(0, 1);
				//remove one space in front of each value comma if it exists
				for (size_t i = 0; i < value.length(); i++)
				{
					if (value[i] == ','
						&& i + 1 < value.length()
						&& value[i + 1] == ' ')
					{
						value.erase(i + 1, 1);
					}
				}

				if (type == "id")
				{
					if (!obj.empty()) LoadGameObject(obj);

					obj.clear();
					obj[type] = value;
				}
				else obj[type] = value;
			}
		}

		levelFile.close();

		if (!obj.empty()) LoadGameObject(obj);

		currentLevelPath = levelPath;

		if (unsavedChanges) Render::SetWindowNameAsUnsaved(false);

		cout << "Successfully loaded levelPath '" << currentLevelPath << "'!\n";
	}

	void LevelFile::LoadGameObject(const map<string, string> obj)
	{
		//
		// VALUES USED FOR ALL GAMEOBJECTS
		//

		unsigned int id = stoul(obj.at("id"));
		string name = obj.at("name");
		auto meshType = magic_enum::enum_cast<Mesh::MeshType>(obj.at("type"));

		vec3 pos{};
		if (obj.count("position"))
		{
			vector<string> vect = StringUtils::Split(obj.at("position"), ',');
			pos = StringUtils::StringToVec3(vect);
		}

		vec3 rot{};
		if (obj.count("rotation"))
		{
			vector<string> vect = StringUtils::Split(obj.at("rotation"), ',');
			rot = StringUtils::StringToVec3(vect);
		}

		vec3 scale{};
		if (obj.count("scale"))
		{
			vector<string> vect = StringUtils::Split(obj.at("scale"), ',');
			scale = StringUtils::StringToVec3(vect);
		}

		vector<string> shaders = StringUtils::Split(obj.at("shaders"), ',');

		//
		// EXTRA VALUES
		//

		vector<string> textures;
		if (obj.count("textures")) textures = StringUtils::Split(obj.at("textures"), ',');

		float shininess = 32;
		if (obj.count("shininess")) shininess = stof(obj.at("shininess"));

		vec3 diffuse{};
		if (obj.count("diffuse"))
		{
			vector<string> vect = StringUtils::Split(obj.at("diffuse"), ',');
			diffuse = StringUtils::StringToVec3(vect);
		}

		float intensity;
		if (obj.count("intensity")) intensity = stof(obj.at("intensity"));

		float distance;
		if (obj.count("distance")) distance = stof(obj.at("distance"));

		float innerAngle;
		if (obj.count("inner angle")) innerAngle = stof(obj.at("inner angle"));

		float outerAngle;
		if (obj.count("outer angle")) outerAngle = stof(obj.at("outer angle"));

		//
		// ATTACHED BILLBOARD VALUES
		//

		string billboardName;
		if (obj.count("billboard name")) billboardName = obj.at("billboard name");

		unsigned int billboardID;
		if (obj.count("billboard id")) billboardID = stoul(obj.at("billboard id"));

		vector<string> billboardShaders{};
		if (obj.count("billboard shaders")) billboardShaders = StringUtils::Split(obj.at("billboard shaders"), ',');

		string billboardDiffTexture;
		if (obj.count("billboard texture")) billboardDiffTexture = obj.at("billboard texture");

		float billboardShininess;
		if (obj.count("billboard shininess")) billboardShininess = stof(obj.at("billboard shininess"));

		//
		// CREATE GAMEOBJECTS
		//

		if (meshType == Mesh::MeshType::cube)
		{
			Cube::InitializeCube(
				pos,
				rot,
				scale,
				shaders[0],
				shaders[1],
				textures[0],
				textures[1],
				shininess,
				name,
				id);
		}
		else if (meshType == Mesh::MeshType::point_light)
		{
			PointLight::InitializePointLight(
				pos,
				rot,
				scale,
				shaders[0],
				shaders[1],
				diffuse,
				intensity,
				distance,
				name,
				id,
				billboardShaders[0],
				billboardShaders[1],
				billboardDiffTexture,
				billboardShininess,
				billboardName,
				billboardID);
		}
		else if (meshType == Mesh::MeshType::spot_light)
		{
			SpotLight::InitializeSpotLight(
				pos,
				rot,
				scale,
				shaders[0],
				shaders[1],
				diffuse,
				intensity,
				distance,
				innerAngle,
				outerAngle,
				name,
				id,
				billboardShaders[0],
				billboardShaders[1],
				billboardDiffTexture,
				billboardShininess,
				billboardName,
				billboardID);
		}
	}

	void LevelFile::SaveLevel(SaveType saveType, const string& targetLevel)
	{
		vector<shared_ptr<GameObject>> objects = GameObjectManager::GetObjects();

		if (currentLevelPath == "")
		{
			cout << "Error: Couldn't save level file '" << currentLevelPath << "' because no level is open!\n";
			return;
		}

		ofstream levelFile(currentLevelPath);

		if (!levelFile.is_open())
		{
			cout << "Error: Couldn't write into level file '" << currentLevelPath << "'!\n";
			return;
		}

		for (const auto& obj : objects)
		{
			//ignore border gameobject
			//billboards and actiontex
			if (obj->GetID() != 10000000
				&& obj->GetID() != 10000001
				&& obj->GetMesh()->GetMeshType() != Mesh::MeshType::billboard)
			{
				levelFile << "id: " << to_string(obj->GetID()) << "\n";

				levelFile << "\n";

				levelFile << "name: " << obj->GetName() << "\n";

				string type = string(magic_enum::enum_name(obj->GetMesh()->GetMeshType()));
				levelFile << "type: " << type << "\n";

				//position
				float posX = obj->GetTransform()->GetPosition().x;
				float posY = obj->GetTransform()->GetPosition().y;
				float posZ = obj->GetTransform()->GetPosition().z;
				levelFile << "position: " << posX << ", " << posY << ", " << posZ << "\n";

				//rotation
				float rotX = obj->GetTransform()->GetRotation().x;
				float rotY = obj->GetTransform()->GetRotation().y;
				float rotZ = obj->GetTransform()->GetRotation().z;
				levelFile << "rotation: " << rotX << ", " << rotY << ", " << rotZ << "\n";

				//scale
				float scaleX = obj->GetTransform()->GetScale().x;
				float scaleY = obj->GetTransform()->GetScale().y;
				float scaleZ = obj->GetTransform()->GetScale().z;
				levelFile << "scale: " << scaleX << ", " << scaleY << ", " << scaleZ << "\n";

				//object textures
				Mesh::MeshType meshType = obj->GetMesh()->GetMeshType();
				if (meshType == Mesh::MeshType::cube)
				{
					string diffuseTexture = obj->GetMaterial()->GetTextureName(0);
					string specularTexture = obj->GetMaterial()->GetTextureName(1);
					levelFile << "textures: " << diffuseTexture << ", " << specularTexture << "\n";
				}

				//shaders
				string vertexShader = obj->GetMaterial()->GetShaderName(0);
				string fragmentShader = obj->GetMaterial()->GetShaderName(1);
				levelFile << "shaders: " << vertexShader << ", " << fragmentShader << "\n";

				//material variables
				if (meshType == Mesh::MeshType::cube)
				{
					levelFile << "shininess: " << obj->GetBasicShape()->GetShininess() << "\n";
				}
				else if (meshType == Mesh::MeshType::point_light)
				{
					float pointDiffuseX = obj->GetPointLight()->GetDiffuse().x;
					float pointDiffuseY = obj->GetPointLight()->GetDiffuse().y;
					float pointDiffuseZ = obj->GetPointLight()->GetDiffuse().z;
					levelFile << "diffuse: " << pointDiffuseX << ", " << pointDiffuseY << ", " << pointDiffuseZ << "\n";

					levelFile << "intensity: " << obj->GetPointLight()->GetIntensity() << "\n";

					levelFile << "distance: " << obj->GetPointLight()->GetDistance() << "\n";
				}
				else if (meshType == Mesh::MeshType::spot_light)
				{
					float spotDiffuseX = obj->GetSpotLight()->GetDiffuse().x;
					float spotDiffuseY = obj->GetSpotLight()->GetDiffuse().y;
					float spotDiffuseZ = obj->GetSpotLight()->GetDiffuse().z;
					levelFile << "diffuse: " << spotDiffuseX << ", " << spotDiffuseY << ", " << spotDiffuseZ << "\n";

					levelFile << "intensity: " << obj->GetSpotLight()->GetIntensity() << "\n";

					levelFile << "distance: " << obj->GetSpotLight()->GetDistance() << "\n";

					levelFile << "inner angle: " << obj->GetSpotLight()->GetInnerAngle() << "\n";

					levelFile << "outer angle: " << obj->GetSpotLight()->GetOuterAngle() << "\n";
				}

				//also save billboard data of each light source
				if (meshType == Mesh::MeshType::point_light
					|| meshType == Mesh::MeshType::spot_light)
				{
					levelFile << "\n";
					levelFile << "---attatched billboard data---" << "\n";
					levelFile << "\n";

					levelFile << "billboard name: " << obj->GetChildBillboard()->GetName() << "\n";

					levelFile << "billboard id: " << obj->GetChildBillboard()->GetID() << "\n";

					string billboardVertShader = obj->GetChildBillboard()->GetMaterial()->GetShaderName(0);
					string billboardFragShader = obj->GetChildBillboard()->GetMaterial()->GetShaderName(1);
					levelFile << "billboard shaders: " << billboardVertShader << ", " << billboardFragShader << "\n";

					levelFile << "billboard texture: " << obj->GetChildBillboard()->GetMaterial()->GetTextureName(0) << "\n";

					levelFile << "billboard shininess: " << obj->GetChildBillboard()->GetBasicShape()->GetShininess() << "\n";
				}

				levelFile << "\n";
				levelFile << "==================================================\n";
				levelFile << "\n";
			}
		}

		levelFile.close();

		if (unsavedChanges) Render::SetWindowNameAsUnsaved(false);

		cout << "\nSuccessfully saved level '" << currentLevelPath + "'!\n";

		switch (saveType)
		{
		case SaveType::sceneSwitch:
			LoadLevel(targetLevel);
			break;
		case SaveType::shutDown:
			LevelEditor::Shutdown();
			break;
		}
	}
}
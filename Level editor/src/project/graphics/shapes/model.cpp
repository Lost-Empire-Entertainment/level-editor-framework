//Copyright (c) <2024> <Lost Empire Entertainment>

#include <iostream>

//external
#include "assimp/postprocess.h"
#include "glm.hpp"
#include "stb_image.h"

//level editor
#include "model.hpp"
#include "core.hpp"

using std::cout;
using glm::vec3;
using glm::vec2;

using Core::LevelEditor;
using MeshType = Graphics::Shape::Mesh::MeshType;

namespace Graphics::Shape
{
	void Model::InitializeModel(const string& path)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(
			path,
			aiProcess_Triangulate
			| aiProcess_GenSmoothNormals
			| aiProcess_FlipUVs
			| aiProcess_CalcTangentSpace);

		if (!scene
			|| scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE
			|| !scene->mRootNode)
		{
			cout << "Error: " << importer.GetErrorString() << "\n";
			return;
		}

		directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(scene->mRootNode, scene);
	}

	void Model::SetUpModel()
	{

	}

	void Model::RenderModel(const shared_ptr<GameObject>& obj, const mat4& view, const mat4& projection)
	{
		
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(ProcessMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	AssimpMesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		vector<AssimpVertex> vertices;
		vector<unsigned int> indices;
		vector<AssimpTexture> textures;

		//each mesh vertice
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			AssimpVertex vertex;
			vec3 vector{};

			//positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.pos = vector;

			//normals
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.normal = vector;
			}

			//texture coordinates
			if (mesh->mTextureCoords[0])
			{
				vec2 vec{};
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;

				//tangent
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.tangent = vector;

				//bitangent
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.bitangent = vector;
			}
			else vertex.texCoords = vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

		//each mesh face
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		vector<AssimpTexture> diffuseMaps = LoadMaterialTextures(
			material, 
			aiTextureType_DIFFUSE, 
			"texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<AssimpTexture> specularMaps = LoadMaterialTextures(
			material,
			aiTextureType_SPECULAR,
			"texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		vector<AssimpTexture> normalMaps = LoadMaterialTextures(
			material,
			aiTextureType_HEIGHT,
			"texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		vector<AssimpTexture> heightMaps = LoadMaterialTextures(
			material,
			aiTextureType_AMBIENT,
			"texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		return AssimpMesh(vertices, indices, textures);
	}

	vector<AssimpTexture> Model::LoadMaterialTextures(
		aiMaterial* mat, 
		aiTextureType type, 
		string typeName)
	{
		vector<AssimpTexture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			bool skip = false;
			for (unsigned int j = 0; j < loadedTextures.size(); j++)
			{
				if (strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(loadedTextures[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				AssimpTexture texture;
				texture.ID = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				loadedTextures.push_back(texture);
			}
		}

		return textures;
	}

	unsigned int Model::TextureFromFile(const char* path, const string& directory, bool gamma)
	{
		string fileName = string(path);
		fileName = directory + '/' + fileName;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format{};
			if (nrComponents == 1) format = GL_RED;
			else if (nrComponents == 3) format = GL_RGB;
			else if (nrComponents == 4) format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			cout << "Error: Failed to load image " << path << "\n";
			stbi_image_free(data);
		}

		return textureID;
	}
}
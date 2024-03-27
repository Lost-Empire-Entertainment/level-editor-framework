//Copyright (c) <2024> <Lost Empire Entertainment>

#include <string>
#include <memory>

//external
#include "assimp/Importer.hpp"
#include "assimp/scene.h"

//level editor
#include "gameobject.hpp"

using std::string;
using std::shared_ptr;

using Graphics::Shape::GameObject;

namespace Graphics::Shape
{
	class Model
	{
	public:
		static inline string tempName = "123456789";
		static inline unsigned int tempID = 123456789;

		static inline vector<AssimpTexture> loadedTextures;
		static inline vector<AssimpMesh> meshes;
		static inline string directory;

		static void InitializeModel(const string& path);

		static void RenderModel(const shared_ptr<GameObject>& obj, const mat4& view, const mat4& projection);
	private:
		static void ProcessNode(aiNode* node, const aiScene* scene);

		static AssimpMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

		static vector<AssimpTexture> LoadMaterialTextures(
			aiMaterial* mat, 
			aiTextureType type, 
			string typeName);

		static unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);

		static void SetUpModel();
	};
}
//Copyright (c) <2024> <Lost Empire Entertainment>

#include <string>
#include <memory>

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

		static shared_ptr<GameObject> InitializeModel(
			const vec3& pos = vec3(0),
			const vec3& rot = vec3(0),
			const vec3& scale = vec3(1),
			const string& vertShader = "shaders/GameObject.vert",
			const string& fragShader = "shaders/GameObject.frag",
			const string& diffTexture = "textures/crate_2.png",
			const string& specTexture = "textures/crate_2_specular.png",
			string& name = tempName,
			unsigned int& id = tempID);

		static void RenderModel(const shared_ptr<GameObject>& obj, const mat4& view, const mat4& projection);
	};
}
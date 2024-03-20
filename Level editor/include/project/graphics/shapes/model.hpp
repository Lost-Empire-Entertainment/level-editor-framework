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

		static shared_ptr<GameObject> InitializeModel();

		static void RenderModel(const shared_ptr<GameObject>& obj, const mat4& view, const mat4& projection);
	};
}
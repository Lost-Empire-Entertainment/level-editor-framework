//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

#include <memory>

//external
#include "glm.hpp"

//engine
#include "gameobject.hpp"

using std::shared_ptr;
using glm::vec3;

using Graphics::Shape::GameObject;

namespace Graphics::Shape
{
	class Cube
	{
	public:
		static inline string tempName = "123456789";
		static inline unsigned int tempID = 123456789;

		static shared_ptr<GameObject> InitializeCube(
			const vec3& pos = vec3(0),
			const vec3& rot = vec3(0),
			const vec3& scale = vec3(1),
			const string& vertShader = "shaders/GameObject.vert",
			const string& fragShader = "shaders/GameObject.frag",
			const string& diffTexture = "textures/crate_2.png",
			const string& specTexture = "textures/crate_2_specular.png",
			const float& shininess = 32,
			string& name = tempName,
			unsigned int& id = tempID);

		static void RenderCube(const shared_ptr<GameObject>& obj, const mat4& view, const mat4& projection);
	};
}
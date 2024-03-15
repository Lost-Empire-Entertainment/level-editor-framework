//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

//external
#include "glm.hpp"

//engine
#include "gameobject.hpp"

using glm::vec3;
using glm::mat4;

using Graphics::Shape::GameObject;

namespace Graphics::Shape
{
	class ActionTex
	{
	public:
		static shared_ptr<GameObject> InitializeActionTex(
			const vec3& pos = vec3(0),
			const vec3& rot = vec3(0),
			const vec3& scale = vec3(1));

		static void RenderActionTex(const shared_ptr<GameObject>& obj, const mat4& view, const mat4& projection);
	};
}
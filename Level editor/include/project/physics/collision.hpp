//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

//engine
#include "selectobject.hpp"
#include "gameobject.hpp"

using Graphics::Shape::GameObject;
using Physics::Select;

namespace Physics
{
	class Collision
	{
	public:
		static bool IsRayIntersectingCube(const Select::Ray& ray, const shared_ptr<GameObject>& cube);
	};
}
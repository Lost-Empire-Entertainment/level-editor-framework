//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

//external
#include "glfw3.h"
#include "glm.hpp"

//game
#include "input.hpp"

using glm::vec3;
using glm::mat4;

using Core::Input;

namespace Graphics
{
	class Render
	{
	public:
		static inline GLFWwindow* window;

		static inline vec3 directionalDirection = vec3(-0.2f, -1.0f, -0.3f);
		static inline vec3 directionalDiffuse = vec3(0.4f, 0.4f, 0.4f);
		static inline float directionalIntensity = 1.0f;

		static inline mat4 projection;
		static inline mat4 view;

		static Core::Input camera;

		/// <summary>
		/// Initialize all the render related libraries.
		/// </summary>
		static void RenderSetup();

		/// <summary>
		/// The main loop of all rendering functions.
		/// </summary>
		static void WindowLoop();
	};
}
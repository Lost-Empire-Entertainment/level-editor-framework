//Copyright (c) <2024> <Greenlaser>

#pragma once

//external
#include "glfw3.h"

namespace Graphics
{
	class Render
	{
	public:
		static inline GLFWwindow* window;

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
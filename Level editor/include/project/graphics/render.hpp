//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

//external
#include "glfw3.h"
#include "glm.hpp"

//level editor
#include "input.hpp"

using glm::vec3;
using glm::mat4;

using Core::Input;

namespace Graphics
{
	class Render
	{
	public:
		static inline unsigned int SCR_WIDTH = 1280;
		static inline unsigned int SCR_HEIGHT = 720;

		static inline vec3 backgroundColor = vec3(0.1f, 0.1f, 0.1f);

		static inline GLFWwindow* window;

		static inline vec3 directionalDirection = vec3(-0.2f, -1.0f, -0.3f);
		static inline vec3 directionalDiffuse = vec3(0.4f, 0.4f, 0.4f);
		static inline float directionalIntensity = 1.0f;

		static inline int windowedWidth;
		static inline int windowedHeight;
		static inline mat4 projection;
		static inline mat4 view;

		static Core::Input camera;

		/// <summary>
		/// Initialize all the render related libraries.
		/// </summary>
		static void RenderSetup();

		/// <summary>
		/// Change rendered content after window scale is adjusted.
		/// </summary>
		/// <param name="window"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		static void UpdateAfterRescale(GLFWwindow* window, int width, int height);

		/// <summary>
		/// Update window title to show unsaved data
		/// </summary>
		static void SetWindowNameAsUnsaved(bool state);

		/// <summary>
		/// The main loop of all rendering functions.
		/// </summary>
		static void WindowLoop();
	private:
		static void GLFWSetup();
		static void WindowSetup();
		static void GladSetup();
		static void ContentSetup();
	};
}
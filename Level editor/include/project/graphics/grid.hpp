//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

//external
#include "glm.hpp"

//engine
#include "shader.hpp"

using glm::vec3;

using Graphics::Shader;

namespace Graphics
{
	class Grid
	{
	public:
		static inline vec3 color = vec3(0.4f, 0.4f, 0.4f);
		static inline float transparency = 0.25f;

		static void InitializeGrid();
		static void RenderGrid(const mat4& view, const mat4& projection);
	private:
		static inline GLuint VAO, VBO;
		static inline Shader shader;

		static inline const int lineCount = 10000; //BREAKS IF YOU PUT IT ABOVE 100000
		static inline float lineDistance = 1.0f; //BREAKS IF YOU PUT IT BELOW 1.0f
		static inline float vertices[lineCount * 4 * 3];
	};
}
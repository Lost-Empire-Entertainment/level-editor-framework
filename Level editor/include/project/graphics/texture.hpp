//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

#include <vector>

//external
#include "glad.h"

//engine
#include "gameobject.hpp"

using std::vector;

using Graphics::Shape::GameObject;

namespace Graphics
{
	class Texture
	{
	public:
		const string texturePath;
		Texture(const string& path);

		/// <summary>
		/// Handles the loading of each game texture.
		/// </summary>
		/// <param name="textureName">What is the actual texture name we are trying to load?</param>
		/// <param name="flipTexture">Should the texture be flipped 180 degrees?</param>
		/// <param name="pixelFormat">What is the pixel format of the texture? (GL_RGB/GL_RGBA etc.)</param>
		void LoadTexture(const shared_ptr<GameObject>& obj, const string& textureName, bool flipTexture = false, GLenum pixelFormat = GL_RGB);
		void DeleteTexture(unsigned int texture)
		{
			glDeleteTextures(1, &texture);
		}
	};
}
//Copyright (c) <2024> <Lost Empire Entertainment>

#include <string>
#include <vector>
#include <iostream>

//external
#include "glfw3.h"
#include "stb_image.h"

//engine
#include "texture.hpp"

using std::cout;
using std::endl;

namespace Graphics
{
	Texture::Texture(const string& path) : texturePath(path){}

	void Texture::LoadTexture(const shared_ptr<GameObject>& obj, const string& textureName, bool flipTexture, GLenum pixelFormat)
	{
		cout << "Initializing texture " << textureName << "...\n";

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		//set texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//load image, create texture and generate mipmaps
		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(flipTexture);
		unsigned char* data = stbi_load((texturePath + "\\" + textureName).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, pixelFormat, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			obj->GetMaterial()->AddTexture(textureName, texture);

			cout << "Texture " << textureName << " initialized successfully!\n\n";
		}
		else
		{
			cout << "Failed to load " << textureName << " texture!\n\n";
		}
		stbi_image_free(data);
	}
}
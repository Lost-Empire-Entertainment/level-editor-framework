//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

#include <string>

//external
#include "glm.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;
using std::string;

namespace Graphics
{
	class Shader
	{
	public:
		unsigned int ID;

		Shader(const string& vertexPath = "", const string& fragmentPath = "");

		void Use() const;

		void SetBool(const string &name, bool value) const;
		void SetInt(const string &name, int value) const;
		void SetFloat(const string &name, float value) const;

		void SetVec2(const string& name, const vec2& value) const;
		void SetVec2(const string& name, float x, float y) const;

		void SetVec3(const string& name, const vec3& value) const;
		void SetVec3(const string& name, float x, float y, float z) const;

		void SetVec4(const string& name, const vec4& value) const;
		void SetVec4(const string& name, float x, float y, float z, float w) const;

		void SetMat2(const string& name, const mat2& mat) const;
		void SetMat3(const string& name, const mat3& mat) const;
		void SetMat4(const string& name, const mat4& mat) const;
	private:
		void CheckCompileErrors(GLuint shader, string type);
	};
}
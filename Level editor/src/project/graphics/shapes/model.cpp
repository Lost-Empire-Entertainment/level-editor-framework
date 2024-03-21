//Copyright (c) <2024> <Lost Empire Entertainment>

//external
#include "assimp/Importer.hpp"

//level editor
#include "model.hpp"
#include "core.hpp"

using Core::LevelEditor;
using MeshType = Graphics::Shape::Mesh::MeshType;

namespace Graphics::Shape
{
	shared_ptr<GameObject> Model::InitializeModel(
		const vec3& pos,
		const vec3& rot,
		const vec3& scale,
		const string& vertShader,
		const string& fragShader,
		const string& diffTexture,
		const string& specTexture,
		string& name,
		unsigned int& id)
	{
		shared_ptr<Transform> transform = make_shared<Transform>(pos, rot, scale);

		shared_ptr<Mesh> mesh = make_shared<Mesh>(MeshType::cube);

		Shader modelShader = Shader(
			LevelEditor::filesPath + "/" + vertShader,
			LevelEditor::filesPath + "/" + fragShader);

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
	}

	void Model::RenderModel(const shared_ptr<GameObject>& obj, const mat4& view, const mat4& projection)
	{

	}
}
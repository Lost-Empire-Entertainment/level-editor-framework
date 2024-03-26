//Copyright (c) <2024> <Lost Empire Entertainment>

//external
#include "glad.h"
#include "quaternion.hpp"
#include "matrix_transform.hpp"

//engine
#include "pointlight.hpp"
#include "shader.hpp"
#include "core.hpp"
#include "render.hpp"
#include "selectobject.hpp"
#include "billboard.hpp"

using glm::translate;
using glm::rotate;
using glm::radians;
using glm::quat;
using glm::scale;

using Graphics::Shader;
using Graphics::Shape::Mesh;
using Type = Graphics::Shape::Mesh::MeshType;
using Graphics::Shape::Material;
using Graphics::Shape::GameObjectManager;
using Core::LevelEditor;
using Graphics::Render;
using Physics::Select;

namespace Graphics::Shape
{
	shared_ptr<GameObject> PointLight::InitializePointLight(
		const vec3& pos,
		const vec3& rot,
		const vec3& scale,
		const string& vertShader,
		const string& fragShader,
		const vec3& diffuse,
		const float& intensity,
		const float& distance,
		string& name,
		unsigned int& id,
		
		const string& billboardVertShader,
		const string& billboardFragShader,
		const string& billboardDiffTexture,
		const float& billboardShininess,
		string& billboardName,
		unsigned int& billboardID)
	{
		shared_ptr<Transform> transform = make_shared<Transform>(pos, rot, scale);

		float vertices[] =
		{
			//edges of the cube
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,

			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,

			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,

			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			//connecting edges
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,

			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,

			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f,  0.5f,
		};

		shared_ptr<Mesh> mesh = make_shared<Mesh>(Type::point_light);

		Shader pointLightShader = Shader(
			LevelEditor::filesPath + "/" + vertShader,
			LevelEditor::filesPath + "/" + fragShader);

		GLuint vao, vbo, ebo;

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		shared_ptr<Material> mat = make_shared<Material>(vao, vbo, ebo);
		mat->AddShader(vertShader, fragShader, pointLightShader);

		shared_ptr<PointLight_Variables> pointLight = 
			make_shared<PointLight_Variables>(
				diffuse, 
				intensity, 
				distance);

		shared_ptr<GameObject> billboard = Billboard::InitializeBillboard(
			pos,
			rot,
			scale,
			billboardVertShader,
			billboardFragShader,
			billboardDiffTexture,
			billboardShininess,
			billboardName,
			billboardID);

		if (name == tempName) name = "Point light";
		if (id == tempID) id = GameObject::nextID++;
		shared_ptr<GameObject> obj = make_shared<GameObject>(
			true, 
			name, 
			id,
			transform, 
			mesh, 
			mat,
			pointLight);

		billboard->SetParentBillboardHolder(obj);
		obj->SetChildBillboard(billboard);

		GameObjectManager::AddGameObject(obj);
		GameObjectManager::AddOpaqueObject(obj);
		GameObjectManager::AddPointLight(obj);

		return obj;
	}

	void PointLight::RenderPointLight(const shared_ptr<GameObject>& obj, const mat4& view, const mat4& projection)
	{
		Shader shader = obj->GetMaterial()->GetShader();

		shader.Use();
		shader.SetMat4("projection", projection);
		shader.SetMat4("view", view);

		float transparency = 
			Select::selectedObj == obj 
			&& Select::isObjectSelected ? 1.0f : 0.5f;
		shader.SetFloat("transparency", transparency);
		shader.SetVec3("color", obj->GetPointLight()->GetDiffuse());

		mat4 model = mat4(1.0f);
		model = translate(model, obj->GetTransform()->GetPosition());
		quat newRot = quat(radians(obj->GetTransform()->GetRotation()));
		model *= mat4_cast(newRot);
		model = scale(model, obj->GetTransform()->GetScale());

		shader.SetMat4("model", model);
		GLuint VAO = obj->GetMaterial()->GetVAO();
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 24);
	}
}
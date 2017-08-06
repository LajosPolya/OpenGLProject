#pragma once

#include "Mesh.h"
#include "Material.h"
#include "InstancedTransform.h"

class InstancedArrayGameObjectImpl
{
public:
	InstancedArrayGameObjectImpl();
	~InstancedArrayGameObjectImpl();

	InstancedArrayGameObjectImpl(GLchar* vertexShader, GLchar* fragmentShader, GLchar* diffuseMapLoc, GLchar* specularMapLoc, GLchar* meshLoc, GLchar* materialLoc, GLchar* transformLoc, GLchar* lightsLoc, Camera * camera, glm::mat4 projection);

	void Draw();

	Shader* getShader();

	Camera* getCamera();

	InstancedTransform* getTransform();

	Texture* getDiffuseMap();
	Texture* getSpecularMap();

	LightsContainer* getLightsContainer();

private:
	Mesh* mesh;

	Material* material;

	Texture* diffuseMap;
	Texture* specularMap;

	Shader* shader;

	Camera* camera;

	InstancedTransform* transform;

	LightsContainer* lightsContainer;

	glm::mat4 projection;


};

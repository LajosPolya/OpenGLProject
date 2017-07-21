#pragma once

#ifndef INSTANCED_GAMEOBJECT_IMPL_H
#define INSTANCED_GAMEOBJECT_IMPL_H

#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "InstancedTransformImpl.h"
#include "TransformImpl.h"

class InstancedGameObjectImpl : public GameObject
{
public:
	InstancedGameObjectImpl();
	~InstancedGameObjectImpl();

	InstancedGameObjectImpl(GLchar* vertexShader, GLchar* fragmentShader, GLchar* diffuseMapLoc, GLchar* specularMapLoc, GLchar* meshLoc, GLchar* materialLoc, GLchar* transformLoc, GLchar* lightsLoc, Camera * camera, glm::mat4 projection);

	void Draw();

	Shader* getShader();

	Camera* getCamera();

	InstancedTransformImpl* getTransform();

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

	InstancedTransformImpl* transform;

	LightsContainer* lightsContainer;

	glm::mat4 projection;


};

#endif // !INSTANCED_GAMEOBJECT_IMPL_H

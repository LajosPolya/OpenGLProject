#pragma once

#ifndef INSTANCED_GAMEOBJECT_IMPL_H
#define INSTANCED_GAMEOBJECT_IMPL_H

#include "GameObject.h"
#include "Mesh.h"
#include "Material.h"
#include "InstancedTransform.h"
#include "TransformImpl.h"

class InstancedGameObjectImpl : public GameObject
{
public:
	InstancedGameObjectImpl();
	~InstancedGameObjectImpl();

	/* In the future have a Constructor which doesn't take the lightsLoc and allow the LightsContainer to be initialized independenty */
	InstancedGameObjectImpl(GLchar* vertexShader, GLchar* fragmentShader, GLchar* diffuseMapLoc, GLchar* specularMapLoc, GLchar* meshLoc, GLchar* materialLoc, GLchar* transformLoc, GLchar* lightsLoc, Camera * camera, glm::mat4 projection);
	InstancedGameObjectImpl(GLchar* vertexShader, GLchar* fragmentShader, GLchar* diffuseMapLoc, GLchar* meshLoc, GLchar* transformLoc, Camera * camera, glm::mat4 projection);
	InstancedGameObjectImpl(GLchar* vertexShader, GLchar* fragmentShader, GLchar* meshLoc, GLchar* transformLoc, Camera * camera, glm::mat4 projection);

	// Instancing Testing
	InstancedGameObjectImpl(GLchar* vertexShader, GLchar* fragmentShader, GLchar* diffuseMapLoc, GLchar* specularMapLoc, GLchar* meshLoc, GLchar* materialLoc, GLchar* transformLoc, GLchar* lightsLoc, Camera * camera, glm::mat4 projection, void * ptr);

	void Draw();

	Shader* getShader();

	Camera* getCamera();

	Transform* getTransform();

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

	Transform* transform;

	LightsContainer* lightsContainer;

	glm::mat4 projection;


};

#endif // !INSTANCED_GAMEOBJECT_IMPL_H

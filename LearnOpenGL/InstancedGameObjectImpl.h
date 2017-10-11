#pragma once

#include "InstancedGameObject.h"
#include "InstancedTransformImpl.h"
#include "GameObjectUtils.h"
#include "GameObjectMemoryManager.h"

class InstancedGameObjectImpl : public InstancedGameObject
{
public:
	InstancedGameObjectImpl();
	~InstancedGameObjectImpl();

	InstancedGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc, std::string specularMapLoc, std::string meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);

	void Draw();

	Shader * getShader();

	Camera * getCamera();

	InstancedTransformImpl * getTransform();

	Texture * getDiffuseMap(GLint i);
	Texture * getSpecularMap(GLint i);

	LightsContainer * getLightsContainer();

private:
	std::vector<Mesh*> mesh;

	Material * material = nullptr;

	Shader * shader = nullptr;

	Camera * camera = nullptr;

	InstancedTransformImpl * transform = nullptr;

	LightsContainer * lightsContainer = nullptr;

	glm::mat4 projection;
};


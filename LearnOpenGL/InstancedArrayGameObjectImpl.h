#pragma once

#include "InstancedGameObject.h"
#include "InstancedArrayTransformImpl.h"


class InstancedArrayGameObjectImpl
{
public:
	InstancedArrayGameObjectImpl();
	~InstancedArrayGameObjectImpl();

	InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc1, std::string specularMapLoc1, std::string meshLoc1, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);

	void Draw();

	Shader * getShader();

	Camera * getCamera();

	InstancedArrayTransformImpl * getTransform();

	Texture * getDiffuseMap(GLint i);
	Texture * getSpecularMap(GLint i);

	LightsContainer * getLightsContainer();

private:
	std::vector<Mesh*> mesh;

	Material * material;

	Shader * shader;

	Camera * camera;

	InstancedArrayTransformImpl * transform;

	LightsContainer * lightsContainer;

	glm::mat4 projection;
};

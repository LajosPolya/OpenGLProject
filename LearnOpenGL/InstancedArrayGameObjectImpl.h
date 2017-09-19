#pragma once

#include "InstancedGameObject.h"
#include "InstancedArrayTransformImpl.h"


class InstancedArrayGameObjectImpl : public InstancedGameObject
{
public:
	InstancedArrayGameObjectImpl();
	~InstancedArrayGameObjectImpl();

	InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * specularMapLoc, GLchar * meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);

	InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc1, GLchar * specularMapLoc1, GLchar * diffuseMapLoc2, GLchar * specularMapLoc2, GLchar * diffuseMapLoc3, GLchar * specularMapLoc3, GLchar * meshLoc1, GLchar * meshLoc2, GLchar * meshLoc3, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);

	void Draw();

	Shader * getShader();

	Camera * getCamera();

	InstancedArrayTransformImpl * getTransform();

	Texture * getDiffuseMap();
	Texture * getSpecularMap();

	Texture * getDiffuseMap(GLint i);
	Texture * getSpecularMap(GLint i);

	LightsContainer * getLightsContainer();

private:
	std::vector<Mesh*> mesh;
	// Mesh * mesh;

	Material * material;

	Texture * diffuseMap;
	Texture * specularMap;

	Shader * shader;

	Camera * camera;

	InstancedArrayTransformImpl * transform;

	LightsContainer * lightsContainer;

	glm::mat4 projection;
};

#pragma once

#include "InstancedGameObject.h"
#include "TransparentTransformImpl.h"

class TransparentGameObjectImpl
{
public:
	TransparentGameObjectImpl();
	~TransparentGameObjectImpl();

	TransparentGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * specularMapLoc, GLchar * meshLoc,  GLchar* materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);
	TransparentGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection);

	void Draw();

	Shader * getShader();

	Camera * getCamera();

	TransparentTransformImpl * getTransform();

	Texture * getDiffuseMap();
	Texture * getSpecularMap();

	LightsContainer * getLightsContainer();

private:
	Mesh * mesh;

	Material * material;

	Texture * diffuseMap;
	Texture * specularMap;

	Shader * shader;

	Camera * camera;

	TransparentTransformImpl * transform;

	LightsContainer * lightsContainer;

	glm::mat4 projection;
};


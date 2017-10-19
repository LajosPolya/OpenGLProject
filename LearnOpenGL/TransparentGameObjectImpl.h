#pragma once

#include <vector>
#include <string>

#include "InstancedGameObject.h"
#include "TransparentTransformImpl.h"
#include "GameObjectUtils.h"
#include "GameObjectMemoryManager.h"

class TransparentGameObjectImpl
{
public:
	TransparentGameObjectImpl();
	~TransparentGameObjectImpl();

	TransparentGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc, std::string specularMapLoc, std::string meshLoc,  GLchar* materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);
	TransparentGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * diffuseMapLoc, GLchar * meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection);

	void Draw();

	Shader * getShader();

	Camera * getCamera();

	TransparentTransformImpl * getTransform();

	Texture * getDiffuseMap(GLint i);
	Texture * getSpecularMap(GLint i);

	LightsContainer * getLightsContainer();

private:
	std::vector<Mesh*> mesh;

	Material * material;

	Shader * shader;

	Camera * camera;

	TransparentTransformImpl * transform;

	LightsContainer * lightsContainer;

	glm::mat4 projection;
};


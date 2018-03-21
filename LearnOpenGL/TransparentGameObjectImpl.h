#pragma once

#include <vector>
#include <string>

#include "TransparentTransformImpl.h"
#include "GameObjectUtils.h"
#include "GameObjectMemoryManager.h"
#include "Camera.h"
#include "Shader.h"

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

	LightsContainer * getLightsContainer();

private:
	std::vector<InstancedMesh*> mesh;

	Material * material;

	Shader * shader;

	Camera * camera;

	TransparentTransformImpl * transform;

	LightsContainer * lightsContainer;

	glm::mat4 projection;

	std::vector<InstancedMesh*> makeMeshes(std::string path, GLuint numInstances, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps);
};


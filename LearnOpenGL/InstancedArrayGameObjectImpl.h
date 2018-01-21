#pragma once

#include "InstancedGameObject.h"
#include "InstancedArrayTransformImpl.h"
#include "GameObjectUtils.h"
#include "GameObjectMemoryManager.h"


class InstancedArrayGameObjectImpl : public InstancedGameObject
{
public:
	InstancedArrayGameObjectImpl();
	~InstancedArrayGameObjectImpl();
	InstancedArrayGameObjectImpl(const InstancedArrayGameObjectImpl & toCopy);

	InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc1, std::string specularMapLoc1, std::string meshLoc1, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);
	InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * geometryShader, std::string diffuseMapLoc1, std::string specularMapLoc1, std::string meshLoc1, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);
	InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc1, std::string specularMapLoc1, std::string meshLoc1, GLchar * materialLoc, std::vector<glm::vec3> &positions, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);
	InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * geometryShader, std::string diffuseMapLoc1, std::string specularMapLoc1, std::string meshLoc1, GLchar * materialLoc, std::vector<glm::vec3> &positions, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);
	InstancedArrayGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, GLchar * geometryShader, std::string diffuseMapLoc1, std::string specularMapLoc1, std::string meshLoc1, GLchar * materialLoc, std::vector<glm::vec3> &positions, GLchar * lightsLoc, Camera * camera, glm::mat4 projection, GLuint primitiveType);

	void Draw();

	Shader * getShader();

	Camera * getCamera();

	InstancedArrayTransformImpl * getTransform();

	Texture * getDiffuseMap(GLint i);
	Texture * getSpecularMap(GLint i);

	LightsContainer * getLightsContainer();

	std::vector<Mesh*> getMeshes();

private:
	std::vector<Mesh*> mesh;

	Material * material = nullptr;

	Shader * shader = nullptr;

	Camera * camera = nullptr;

	InstancedArrayTransformImpl * transform = nullptr;

	LightsContainer * lightsContainer = nullptr;

	glm::mat4 projection;
};

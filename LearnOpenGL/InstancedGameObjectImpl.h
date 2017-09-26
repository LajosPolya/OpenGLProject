#pragma once

#include "InstancedGameObject.h"
#include "InstancedTransformImpl.h"
#include "GameObjectUtils.h"

class InstancedGameObjectImpl : public InstancedGameObject
{
public:
	InstancedGameObjectImpl();
	~InstancedGameObjectImpl();

	InstancedGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc, std::string specularMapLoc, std::string meshLoc, GLchar * materialLoc, GLchar * transformLoc, GLchar * lightsLoc, Camera * camera, glm::mat4 projection);
	InstancedGameObjectImpl(GLchar * vertexShader, GLchar * fragmentShader, std::string diffuseMapLoc, std::string meshLoc, GLchar * transformLoc, Camera * camera, glm::mat4 projection);

	void Draw();

	Shader* getShader();

	Camera* getCamera();

	InstancedTransformImpl* getTransform();

	Texture* getDiffuseMap(GLint i);
	Texture* getSpecularMap(GLint i);

	LightsContainer* getLightsContainer();

private:
	// Mesh* mesh;
	std::vector<Mesh*> mesh;

	Material* material;

	Shader* shader;

	Camera* camera;

	InstancedTransformImpl* transform;

	LightsContainer* lightsContainer;

	glm::mat4 projection;
};


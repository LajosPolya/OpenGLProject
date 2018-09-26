#pragma once

#include "InstancedArrayMesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "LightsContainer.h"
#include "GameObjectUtils.h"

class SimpleInstancedArrayGameObject
{
public:
	SimpleInstancedArrayGameObject(const SimpleInstancedArrayGameObject & toCopy);
	~SimpleInstancedArrayGameObject();

	SimpleInstancedArrayGameObject();
	SimpleInstancedArrayGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::string transformPath);
	SimpleInstancedArrayGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::vector<glm::vec3> positions);
	SimpleInstancedArrayGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::vector<glm::vec3> positions, GLuint primitiveType);

	void Draw();

	InstancedArrayTransformImpl * getTransform();

	void setInstances(InstancedArrayTransformImpl * transform);

private:
	std::vector<InstancedArrayMesh*> mesh;

	InstancedArrayTransformImpl * transform = nullptr;

	std::vector<InstancedArrayMesh*> makeMeshes(std::string path, std::vector<glm::mat4> models, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps);
	std::vector<InstancedArrayMesh*> makeMeshes(std::string path, std::vector<glm::mat4> models, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps, GLuint primitiveType);
};


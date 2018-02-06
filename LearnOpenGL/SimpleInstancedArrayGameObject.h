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
	SimpleInstancedArrayGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::string materialPath, std::string transformPath);
	SimpleInstancedArrayGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::string materialPath, std::vector<glm::vec3> positions);
	SimpleInstancedArrayGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::string materialPath, std::vector<glm::vec3> positions, GLuint primitiveType);

	void Draw();

	InstancedArrayTransformImpl * getTransform();

	Material * getMaterial();

	void setInstances(InstancedArrayTransformImpl * transform);

private:
	std::vector<InstancedArrayMesh*> mesh;

	Material * material = nullptr;

	InstancedArrayTransformImpl * transform = nullptr;
};


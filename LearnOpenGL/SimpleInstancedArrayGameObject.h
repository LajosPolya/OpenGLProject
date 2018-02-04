#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "LightsContainer.h"
#include "GameObjectUtils.h"

class SimpleInstancedArrayGameObject
{
public:
	SimpleInstancedArrayGameObject();
	SimpleInstancedArrayGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, GLchar * materialPath, std::vector<glm::vec3> positions);
	SimpleInstancedArrayGameObject(const SimpleInstancedArrayGameObject & toCopy);
	~SimpleInstancedArrayGameObject();

	void Draw();

	InstancedArrayTransformImpl * getTransform();

	Material * getMaterial();

	void setInstances(InstancedArrayTransformImpl * transform);

private:
	std::vector<Mesh*> mesh;

	Material * material = nullptr;

	InstancedArrayTransformImpl * transform = nullptr;
};


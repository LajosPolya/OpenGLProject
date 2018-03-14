#pragma once

#include <vector>
#include <string>

#include "InstancedMesh.h"
#include "Material.h"
#include "GameObjectUtils.h"


class SimpleInstancedGameObject
{
public:
	SimpleInstancedGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::string transformPath);
	SimpleInstancedGameObject(std::string meshPath, InstancedTransformImpl * transform);
	~SimpleInstancedGameObject();

	InstancedTransformImpl * getTransform();

	void Draw();

	void setInstances(InstancedTransformImpl * transform);
	GLuint hasInstancesChangedAndSetFalse();

private:
	GLuint instancesUpdated = 1;

	std::vector<InstancedMesh*> mesh;

	InstancedTransformImpl * transform = nullptr;
};


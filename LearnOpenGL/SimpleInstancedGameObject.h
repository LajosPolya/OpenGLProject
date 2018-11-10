#pragma once

#include <vector>
#include <string>

#include "InstancedTransformImpl.h"
#include "InstancedMesh.h"
#include "Material.h"
#include "GameObjectUtils.h"


class SimpleInstancedGameObject
{
public:
	SimpleInstancedGameObject(string diffuseMapPath, string specularMapPath, string meshPath, string transformPath);
	SimpleInstancedGameObject(string meshPath, InstancedTransformImpl * transform);
	~SimpleInstancedGameObject();

	InstancedTransformImpl * getTransform();

	void Draw();

	void setInstances(InstancedTransformImpl * transform);
	GLuint hasInstancesChangedAndSetFalse();

private:
	GLuint instancesUpdated = 1;

	vector<InstancedMesh*> mesh;

	InstancedTransformImpl * transform = nullptr;

	vector<InstancedMesh*> makeMeshes(string path, GLuint numInstances, vector<Texture*> diffuseMaps, vector<Texture*> specularMaps);
};


#pragma once

#include "InstancedArrayMesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "LightsContainer.h"
#include "GameObjectUtils.h"

using namespace std;
using namespace glm;

class SimpleInstancedArrayGameObject
{
public:
	SimpleInstancedArrayGameObject(const SimpleInstancedArrayGameObject & toCopy);
	~SimpleInstancedArrayGameObject();

	SimpleInstancedArrayGameObject();
	SimpleInstancedArrayGameObject(string diffuseMapPath, string specularMapPath, string meshPath, string transformPath);
	SimpleInstancedArrayGameObject(string diffuseMapPath, string specularMapPath, string meshPath, vector<vec3> positions);
	SimpleInstancedArrayGameObject(string diffuseMapPath, string specularMapPath, string meshPath, vector<vec3> positions, GLuint primitiveType);

	void Draw();

	InstancedArrayTransformImpl * getTransform();

	void setInstances(InstancedArrayTransformImpl * transform);

private:
	vector<InstancedArrayMesh*> mesh;

	InstancedArrayTransformImpl * transform = nullptr;

	vector<InstancedArrayMesh*> makeMeshes(string path, vector<mat4> models, vector<Texture*> diffuseMaps, vector<Texture*> specularMaps);
	vector<InstancedArrayMesh*> makeMeshes(string path, vector<mat4> models, vector<Texture*> diffuseMaps, vector<Texture*> specularMaps, GLuint primitiveType);
};


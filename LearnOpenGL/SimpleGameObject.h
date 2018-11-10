#pragma once

#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "GameObjectUtils.h"
#include "TransformImpl.h"

using namespace std;

class SimpleGameObject
{
public:
	SimpleGameObject(string meshPath, string transformPath);
	SimpleGameObject(string diffuseMapPath, string meshPath, string transformPath);
	SimpleGameObject(string diffuseMapPath, string specularMapPath, string meshPath, string materialPath, string transformPath);
	SimpleGameObject(string meshPath, Transform * transform);
	~SimpleGameObject();

	void Draw();

	Transform * getTransform();

	// TODO: Add this
	/// GLuint hasInstancesChangedAndSetFalse();

private:
	vector<Mesh*> mesh;

	Transform * transform = nullptr;

	vector<Mesh*> makeMeshes(string path, vector<Texture*> diffuseMaps, vector<Texture*> specularMaps);
};


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
	SimpleGameObject(const string meshPath, const string transformPath);
	SimpleGameObject(const string diffuseMapPath, const string meshPath, const string transformPath);
	SimpleGameObject(const string diffuseMapPath, const string specularMapPath, const string meshPath, const string materialPath, const string transformPath);
	SimpleGameObject(const string meshPath, Transform * transform);
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


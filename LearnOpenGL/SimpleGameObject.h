#pragma once

#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "GameObjectUtils.h"
#include "TransformImpl.h"

class SimpleGameObject
{
public:
	SimpleGameObject(std::string meshPath, std::string transformPath);
	SimpleGameObject(std::string diffuseMapPath, std::string meshPath, std::string transformPath);
	SimpleGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::string materialPath, std::string transformPath);
	SimpleGameObject(std::string meshPath, Transform * transform);
	~SimpleGameObject();

	void Draw();

	Transform * getTransform();

	// TODO: Add this
	/// GLuint hasInstancesChangedAndSetFalse();

private:
	std::vector<Mesh*> mesh;

	Transform * transform = nullptr;

	std::vector<Mesh*> makeMeshes(std::string path, std::vector<Texture*> diffuseMaps, std::vector<Texture*> specularMaps);
};


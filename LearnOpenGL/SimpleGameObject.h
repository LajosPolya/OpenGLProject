#pragma once

#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "GameObjectUtils.h"

class SimpleGameObject
{
public:
	SimpleGameObject(std::string meshPath, std::string transformPath);
	SimpleGameObject(std::string diffuseMapPath, std::string meshPath, std::string transformPath);
	SimpleGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::string materialPath, std::string transformPath);
	SimpleGameObject(std::string meshPath, Transform * transform);
	~SimpleGameObject();

	void Draw();

	Material * getMaterial();

	Transform * getTransform();

	// TODO: Add this
	/// GLuint hasInstancesChangedAndSetFalse();

private:
	std::vector<Mesh*> mesh;

	Material * material = nullptr;

	Transform * transform = nullptr;
};


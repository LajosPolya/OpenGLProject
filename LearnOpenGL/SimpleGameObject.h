#pragma once

#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include "GameObjectUtils.h"

class SimpleGameObject
{
public:
	SimpleGameObject(std::string diffuseMapPath, std::string specularMapPath, std::string meshPath, std::string materialPath, std::string transformPath);
	~SimpleGameObject();

private:
	std::vector<Mesh*> mesh;

	Material * material = nullptr;

	Transform * transform = nullptr;
};


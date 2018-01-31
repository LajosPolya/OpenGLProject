#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "LightsContainer.h"

class SimpleInstancedArrayGameObject
{
public:
	SimpleInstancedArrayGameObject();
	~SimpleInstancedArrayGameObject();

private:
	std::vector<Mesh*> mesh;

	Material * material = nullptr;

	InstancedArrayTransformImpl * transform = nullptr;
};


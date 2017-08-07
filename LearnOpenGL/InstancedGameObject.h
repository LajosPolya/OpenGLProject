#pragma once

#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Camera.h"
#include "InstancedTransform.h"

class InstancedGameObject {
public:
	virtual void Draw() = 0;

	virtual Shader* getShader() = 0;

	virtual Camera* getCamera() = 0;

	virtual InstancedTransform* getTransform() = 0;

	virtual Texture* getDiffuseMap() = 0;
	virtual Texture* getSpecularMap() = 0;

	virtual LightsContainer* getLightsContainer() = 0;
};
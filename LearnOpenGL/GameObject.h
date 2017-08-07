#pragma once

#include "Shader.h"
#include "Transform.h"
#include "Texture.h"
#include "Camera.h"

class GameObject {
public:
	virtual void Draw() = 0;

	virtual Shader* getShader() = 0;

	virtual Camera* getCamera() = 0;

	virtual Transform* getTransform() = 0;

	virtual Texture* getDiffuseMap() = 0;
	virtual Texture* getSpecularMap() = 0;

	virtual LightsContainer* getLightsContainer() = 0;
};

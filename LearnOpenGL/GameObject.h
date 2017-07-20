#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Shader.h"
#include "Transform.h"
#include "Texture.h"

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

#endif // !GAMEOBJECT_H

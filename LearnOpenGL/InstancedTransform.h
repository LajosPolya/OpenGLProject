#pragma once

#include "TransformInt.h"

#include <iostream>

class GameObject;

class InstancedTransform : public TransformInt
{
public:
	InstancedTransform();
	~InstancedTransform();


	InstancedTransform(GLchar* fileLocation, GameObject* gameObject);

	glm::vec3 getPosition();

	void setPosition();
	void setPosition(glm::vec3 pos);

	void setRotation();
	void setYRotation(GLfloat radians);

	void setScale();

	glm::mat4 getModel();

	void refreshModel();

	void Draw();
};


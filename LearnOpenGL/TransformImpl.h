#pragma once

#ifndef TRANSFORM_IMPL_H
#define TRANSFORM_IMPL_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

#include "Transform.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <map>

class GameObject;

class TransformImpl : public Transform {

public:

	TransformImpl();

	TransformImpl(GLchar* fileLocation, GameObject* gameObject);

	glm::vec3 getPosition();

	void setPosition();
	void setPosition(glm::vec3 pos);

	void setRotation();
	void setYRotation(GLfloat radians);

	void setScale();

	glm::mat4 getModel();

	void refreshModel();

	void Draw();

private:
	/* Reference to own GameObject instance*/
	GameObject* gameObject;


	/* Only the file should set this */
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	/* These get updated which update the model */
	glm::vec3 newPosition;
	glm::vec3 newRotation;
	glm::vec3 newScale;

	glm::mat4 model;

	void readFile(GLchar* filename);
};

#endif // !TRANSFORM_IMPL_H